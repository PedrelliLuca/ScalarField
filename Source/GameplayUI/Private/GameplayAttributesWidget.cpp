// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAttributesWidget.h"

#include "HealthComponent.h"
#include "ManaComponent.h"

void UGameplayAttributesWidget::SetPawn(const TWeakObjectPtr<APawn> pawn) {
    _healthC = pawn->FindComponentByClass<UHealthComponent>();
    if (!_healthC.IsValid()) {
        UE_LOG(LogTemp, Error, TEXT(__FUNCTION__ ": missing UHealthComponent"));
    }

    _manaC = pawn->FindComponentByClass<UManaComponent>();
    if (!_manaC.IsValid()) {
        UE_LOG(LogTemp, Error, TEXT(__FUNCTION__ ": missing UManaComponent"));
    }
}

void UGameplayAttributesWidget::ForgetCurrentPawn() {
    _unbindAll();
    _healthC = nullptr;
    _manaC = nullptr;
}

void UGameplayAttributesWidget::Show() {
    _bindAll();
}

void UGameplayAttributesWidget::Hide() {
    _unbindAll();
}

void UGameplayAttributesWidget::_bindAll() {
    if (_healthC.IsValid()) {
        _setCurrentHealth(_healthC->GetCurrentHealth());
        _setMaxHealth(_healthC->GetMaxHealth());
        _setHealthRegen(_healthC->GetHealthRegen());

        _healthChangedHandle = _healthC->OnHealthChanged().AddUObject(this, &UGameplayAttributesWidget::_setCurrentHealth);
        _maxHealthChangedHandle = _healthC->OnMaxHealthChanged().AddUObject(this, &UGameplayAttributesWidget::_setMaxHealth);
        _healthRegenChangedHandle = _healthC->OnHealthRegenChanged().AddUObject(this, &UGameplayAttributesWidget::_setHealthRegen);
    }

    if (_manaC.IsValid()) {
        _setCurrentMana(_manaC->GetCurrentMana());
        _setMaxMana(_manaC->GetMaxMana());
        _setManaRegen(_manaC->GetManaRegen());

        _manaChangedHandle = _manaC->OnManaChanged().AddUObject(this, &UGameplayAttributesWidget::_setCurrentMana);
        _maxManaChangedHandle = _manaC->OnMaxManaChanged().AddUObject(this, &UGameplayAttributesWidget::_setMaxMana);
        _manaRegenChangedHandle = _manaC->OnManaRegenChanged().AddUObject(this, &UGameplayAttributesWidget::_setManaRegen);
    }
}

void UGameplayAttributesWidget::_unbindAll() {
    if (_healthC.IsValid()) {
        _healthC->OnHealthChanged().Remove(_healthChangedHandle);
        _healthC->OnMaxHealthChanged().Remove(_maxHealthChangedHandle);
        _healthC->OnHealthRegenChanged().Remove(_healthRegenChangedHandle);
    }

    if (_manaC.IsValid()) {
        _manaC->OnManaChanged().Remove(_manaChangedHandle);
        _manaC->OnMaxManaChanged().Remove(_maxManaChangedHandle);
        _manaC->OnManaRegenChanged().Remove(_manaRegenChangedHandle);
    }
}

void UGameplayAttributesWidget::_setMaxHealth(const float newMaxHealth) {
    _maxHealth = newMaxHealth;
    _setHealth();
}

void UGameplayAttributesWidget::_setCurrentHealth(const float newCurrentHealth) {
    _currentHealth = newCurrentHealth;
    _setHealth();
}

void UGameplayAttributesWidget::_setMaxMana(const float newMaxMana) {
    _maxMana = newMaxMana;
    _setMana();
}

void UGameplayAttributesWidget::_setCurrentMana(const float newCurrentMana) {
    _currentMana = newCurrentMana;
    _setMana();
}
