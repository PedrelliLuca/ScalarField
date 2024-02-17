// Fill out your copyright notice in the Description page of Project Settings.

#include "HUDWidget.h"

#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "ManaComponent.h"
#include "TacticalPauseWorldSubsystem.h"
#include "ThermodynamicsInteractorComponent.h"
#include "ThermodynamicsPresenterComponent.h"

void UHUDWidget::SetPawn(TWeakObjectPtr<APawn> pawn) {
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

    _thermoPresC = pawn->FindComponentByClass<UThermodynamicsPresenterComponent>();
    if (!_thermoPresC.IsValid()) {
        UE_LOG(LogTemp, Error, TEXT("%s: missing UThermodynamicsPresenterComponent"), *FString(__FUNCTION__));
    }
}

void UHUDWidget::ForgetCurrentPawn() {
    UnbindCurrentPawn();
    _healthC = nullptr;
    _manaC = nullptr;
    _thermoIntC = nullptr;
}

void UHUDWidget::BindCurrentPawn() {
    UnbindCurrentPawn();

    if (_healthC.IsValid()) {
        _setCurrentHealth(_healthC->GetCurrentHealth());
        _setMaxHealth(_healthC->GetMaxHealth());
        _setHealthRegen(_healthC->GetHealthRegen());

        _healthChangedHandle = _healthC->OnHealthChanged().AddUObject(this, &UHUDWidget::_setCurrentHealth);
        _maxHealthChangedHandle = _healthC->OnMaxHealthChanged().AddUObject(this, &UHUDWidget::_setMaxHealth);
        _healthRegenChangedHandle = _healthC->OnHealthRegenChanged().AddUObject(this, &UHUDWidget::_setHealthRegen);
        _onDeathHandle = _healthC->OnDeath().AddUObject(this, &UHUDWidget::_onDeath);
    }

    if (_manaC.IsValid()) {
        _setCurrentMana(_manaC->GetCurrentMana());
        _setMaxMana(_manaC->GetMaxMana());
        _setManaRegen(_manaC->GetManaRegen());

        _manaChangedHandle = _manaC->OnManaChanged().AddUObject(this, &UHUDWidget::_setCurrentMana);
        _maxManaChangedHandle = _manaC->OnMaxManaChanged().AddUObject(this, &UHUDWidget::_setMaxMana);
        _manaRegenChangedHandle = _manaC->OnManaRegenChanged().AddUObject(this, &UHUDWidget::_setManaRegen);
    }

    if (_thermoIntC.IsValid()) {
        _onTemperatureChange(_thermoIntC->GetTemperature());

        _temperatureChangedHandle = _thermoIntC->OnTemperatureChanged.AddUObject(this, &UHUDWidget::_onTemperatureChange);
    }

    check(!_pauseToggleHandle.IsValid());
    const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
    _setPauseStatus(pauseSubsys->IsTacticalPauseOn());
    _pauseToggleHandle = pauseSubsys->OnTacticalPauseToggle().AddUObject(this, &UHUDWidget::_onTacticalPauseToggle);
}

void UHUDWidget::UnbindCurrentPawn() {
    if (_healthC.IsValid()) {
        _healthC->OnHealthChanged().Remove(_healthChangedHandle);
        _healthC->OnMaxHealthChanged().Remove(_maxHealthChangedHandle);
        _healthC->OnHealthRegenChanged().Remove(_healthRegenChangedHandle);
        _healthC->OnDeath().Remove(_onDeathHandle);
    }

    if (_manaC.IsValid()) {
        _manaC->OnManaChanged().Remove(_manaChangedHandle);
        _manaC->OnMaxManaChanged().Remove(_maxManaChangedHandle);
        _manaC->OnManaRegenChanged().Remove(_manaRegenChangedHandle);
    }

    if (_thermoIntC.IsValid()) {
        _thermoIntC->OnTemperatureChanged.Remove(_temperatureChangedHandle);
    }

    if (_pauseToggleHandle.IsValid()) {
        const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
        pauseSubsys->OnTacticalPauseToggle().Remove(_pauseToggleHandle);
        _pauseToggleHandle.Reset();
    }
}

void UHUDWidget::Show() {
    AddToViewport();
}

void UHUDWidget::Hide() {
    RemoveFromParent();
}

void UHUDWidget::_setMaxHealth(float newMaxHealth) {
    _maxHealth = newMaxHealth;
    _setHealth();
}

void UHUDWidget::_setCurrentHealth(float newCurrentHealth) {
    _currentHealth = newCurrentHealth;
    _setHealth();
}

// The parameter of this function is useless. In fact, this entire funciton shouldn't exist, UnbindCurrentPawn() should be the function bound to
// UHealthComponent::OnDeath(). However, due to an architectural error (see description of FOnDeath macro, UHealthComponent.h), it is necessary. I shall
// remove this one day.
void UHUDWidget::_onDeath(TObjectPtr<AActor> _) {
    UnbindCurrentPawn();
}

void UHUDWidget::_setMaxMana(double newMaxMana) {
    _maxMana = newMaxMana;
    _setMana();
}

void UHUDWidget::_setCurrentMana(double newCurrentMana) {
    _currentMana = newCurrentMana;
    _setMana();
}

void UHUDWidget::_onTacticalPauseToggle(const bool bIsTacticalPauseOn, double) {
    _setPauseStatus(bIsTacticalPauseOn);
}

void UHUDWidget::_onTemperatureChange(const float newTemperture) {
    auto temperatureColor = FLinearColor::Black;

    if (_thermoPresC.IsValid()) {
        temperatureColor = _thermoPresC->GetTemperatureColor();
    }
    _setTemperature(newTemperture, temperatureColor);
}
