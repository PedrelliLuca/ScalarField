// Fill out your copyright notice in the Description page of Project Settings.

#include "HUDWidget.h"

#include "Colorizer.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "ManaComponent.h"
#include "TacticalPauseWorldSubsystem.h"
#include "ThermodynamicComponent.h"

void UHUDWidget::SetPawn(TWeakObjectPtr<APawn> pawn) {
    ForgetCurrentPawn();

    _healthC = pawn->FindComponentByClass<UHealthComponent>();
    check(_healthC.IsValid());

    _manaC = pawn->FindComponentByClass<UManaComponent>();
    check(_manaC.IsValid());

    _thermoC = pawn->FindComponentByClass<UThermodynamicComponent>();
    check(_thermoC.IsValid());
}

void UHUDWidget::ForgetCurrentPawn() {
    UnbindCurrentPawn();
    _healthC = nullptr;
    _manaC = nullptr;
    _thermoC = nullptr;
}

void UHUDWidget::BindCurrentPawn() {
    UnbindCurrentPawn();

    check(_healthC.IsValid()) _setCurrentHealth(_healthC->GetCurrentHealth());
    _setMaxHealth(_healthC->GetMaxHealth());
    _setHealthRegen(_healthC->GetHealthRegen());
    _healthChangedHandle = _healthC->OnHealthChanged().AddUObject(this, &UHUDWidget::_setCurrentHealth);
    _maxHealthChangedHandle = _healthC->OnMaxHealthChanged().AddUObject(this, &UHUDWidget::_setMaxHealth);
    _healthRegenChangedHandle = _healthC->OnHealthRegenChanged().AddUObject(this, &UHUDWidget::_setHealthRegen);

    check(_manaC.IsValid());
    _setCurrentMana(_manaC->GetCurrentMana());
    _setMaxMana(_manaC->GetMaxMana());
    _setManaRegen(_manaC->GetManaRegen());
    _manaChangedHandle = _manaC->OnManaChanged().AddUObject(this, &UHUDWidget::_setCurrentMana);
    _maxManaChangedHandle = _manaC->OnMaxManaChanged().AddUObject(this, &UHUDWidget::_setMaxMana);
    _manaRegenChangedHandle = _manaC->OnManaRegenChanged().AddUObject(this, &UHUDWidget::_setManaRegen);

    check(_thermoC.IsValid());
    _setTemperature(_thermoC->GetTemperature(), FColorizer::GenerateColorFromTemperature(_thermoC->GetTemperature()));
    _temperatureChangedHandle = _thermoC->OnTemperatureChanged.AddUObject(this, &UHUDWidget::_onTemperatureChange);

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
    }

    if (_manaC.IsValid()) {
        _manaC->OnManaChanged().Remove(_manaChangedHandle);
        _manaC->OnMaxManaChanged().Remove(_maxManaChangedHandle);
        _manaC->OnManaRegenChanged().Remove(_manaRegenChangedHandle);
    }

    if (_thermoC.IsValid()) {
        _thermoC->OnTemperatureChanged.Remove(_temperatureChangedHandle);
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

void UHUDWidget::_setMaxHealth(double newMaxHealth) {
    _maxHealth = newMaxHealth;
    _setHealth();
}

void UHUDWidget::_setCurrentHealth(double newCurrentHealth) {
    _currentHealth = newCurrentHealth;
    _setHealth();
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

void UHUDWidget::_onTemperatureChange(const double newTemperture) {
    _setTemperature(newTemperture, FColorizer::GenerateColorFromTemperature(newTemperture));
}
