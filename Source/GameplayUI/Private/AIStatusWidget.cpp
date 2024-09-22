// Fill out your copyright notice in the Description page of Project Settings.

#include "AIStatusWidget.h"

#include "HealthComponent.h"
#include "ManaComponent.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicsInteractorComponent.h"

void UAIStatusWidget::SetPawn(TWeakObjectPtr<APawn> pawn) {
    ForgetCurrentPawn();

    _healthC = pawn->FindComponentByClass<UHealthComponent>();
    if (!_healthC.IsValid()) {
        UE_LOG(LogTemp, Error, TEXT("%s: missing UHealthComponent"), *FString(__FUNCTION__));
    }

    _manaC = pawn->FindComponentByClass<UManaComponent>();
    if (!_manaC.IsValid()) {
        UE_LOG(LogTemp, Error, TEXT("%s: missing UManaComponent"), *FString(__FUNCTION__));
    }

    _thermoIntC = pawn->FindComponentByClass<UThermodynamicsInteractorComponent>();
    if (!_thermoIntC.IsValid()) {
        UE_LOG(LogTemp, Error, TEXT("%s: missing UThermodynamicsInteractorComponent"), *FString(__FUNCTION__));
    }

    UTemperatureDamageHandlerComponent* tempDmgHandlerC = pawn->FindComponentByClass<UTemperatureDamageHandlerComponent>();
    if (IsValid(tempDmgHandlerC)) {
        _comfortUpperLimit = tempDmgHandlerC->GetMaxComfortTemperature();
        _comfortLowerLimit = tempDmgHandlerC->GetMinComfortTemperature();
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: missing UTemperatureDamagehandlerComponent"), *FString(__FUNCTION__));
    }

    _bindAll();
}

void UAIStatusWidget::ForgetCurrentPawn() {
    _unbindAll();
    _healthC = nullptr;
    _manaC = nullptr;
    _thermoIntC = nullptr;
    _comfortLowerLimit = 0.0f;
    _comfortUpperLimit = TNumericLimits<float>::Max();
}

void UAIStatusWidget::Show() {
    // TODO: bind to some input so that player can choose to see AI Status?
}

void UAIStatusWidget::Hide() {
    // TODO: bind to some input so that player can choose to hide AI Status?
}

void UAIStatusWidget::SetComfortIntervalLimits(const float upperLimit, const float lowerLimit) {
    _comfortUpperLimit = upperLimit;
    _comfortLowerLimit = lowerLimit;
}

void UAIStatusWidget::_setCurrentTemperature(const float newTemperature) {
    _updateThermometer(newTemperature);
}

void UAIStatusWidget::_setCurrentHealth(const float newHealth) {
    _currentHealth = newHealth;
    _updateHealth();
}

void UAIStatusWidget::_setMaxHealth(const float newMaxHealth) {
    _maxHealth = newMaxHealth;
    _updateHealth();
}

// The parameter of this function is useless. In fact, this entire funciton shouldn't exist, UnbindCurrentPawn() should be the function bound to
// UHealthComponent::OnDeath(). However, due to an architectural error (see description of FOnDeath macro, UHealthComponent.h), it is necessary. I shall
// remove this one day.
void UAIStatusWidget::_onDeath(TObjectPtr<AActor> _) {
    _unbindAll();
}

void UAIStatusWidget::_setCurrentMana(const float newMana) {
    _currentMana = newMana;
    _updateMana();
}

void UAIStatusWidget::_setMaxMana(const float newMaxMana) {
    _maxMana = newMaxMana;
    _updateMana();
}

void UAIStatusWidget::_bindAll() {
    if (_healthC.IsValid()) {
        _setCurrentHealth(_healthC->GetCurrentHealth());
        _setMaxHealth(_healthC->GetMaxHealth());

        _healthChangedHandle = _healthC->OnHealthChanged().AddUObject(this, &UAIStatusWidget::_setCurrentHealth);
        _maxHealthChangedHandle = _healthC->OnMaxHealthChanged().AddUObject(this, &UAIStatusWidget::_setMaxHealth);
        _onDeathHandle = _healthC->OnDeath().AddUObject(this, &UAIStatusWidget::_onDeath);
    }

    if (_manaC.IsValid()) {
        _setCurrentMana(_manaC->GetCurrentMana());
        _setMaxMana(_manaC->GetMaxMana());

        _manaChangedHandle = _manaC->OnManaChanged().AddUObject(this, &UAIStatusWidget::_setCurrentMana);
        _maxManaChangedHandle = _manaC->OnMaxManaChanged().AddUObject(this, &UAIStatusWidget::_setMaxMana);
    }

    if (_thermoIntC.IsValid()) {
        _setCurrentTemperature(_thermoIntC->GetTemperature());

        _temperatureChangedHandle = _thermoIntC->OnTemperatureChanged.AddUObject(this, &UAIStatusWidget::_setCurrentTemperature);
    }
}

void UAIStatusWidget::_unbindAll() {
    if (_healthC.IsValid()) {
        _healthC->OnHealthChanged().Remove(_healthChangedHandle);
        _healthC->OnMaxHealthChanged().Remove(_maxHealthChangedHandle);
        _healthC->OnDeath().Remove(_onDeathHandle);
    }

    if (_manaC.IsValid()) {
        _manaC->OnManaChanged().Remove(_manaChangedHandle);
        _manaC->OnMaxManaChanged().Remove(_maxManaChangedHandle);
    }

    if (_thermoIntC.IsValid()) {
        _thermoIntC->OnTemperatureChanged.Remove(_temperatureChangedHandle);
    }
}
