// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AIStatusWidget.h"

void UAIStatusWidget::SetComfortIntervalLimits(const float upperLimit, const float lowerLimit) {
    _comfortUpperLimit = upperLimit;
    _comfortLowerLimit = lowerLimit;
}

void UAIStatusWidget::OnTemperatureUpdate(const double newTemperature) {
    _updateThermometer(newTemperature);
}

void UAIStatusWidget::OnHealthUpdate(const double newHealth) {
    _currentHealth = newHealth;
    _updateHealth();
}

void UAIStatusWidget::OnMaxHealthUpdate(const double newMaxHealth) {
    _maxHealth = newMaxHealth;
    _updateHealth();
}

void UAIStatusWidget::OnManaUpdate(const double newMana) {
    _currentMana = newMana;
    _updateMana();
}

void UAIStatusWidget::OnMaxManaUpdate(const double newMaxMana) {
    _maxMana = newMaxMana;
    _updateMana();
}
