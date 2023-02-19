// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AITemperatureWidget.h"

void UAITemperatureWidget::SetComfortIntervalLimits(const float upperLimit, const float lowerLimit) {
	_comfortUpperLimit = upperLimit;
	_comfortLowerLimit = lowerLimit;
}

void UAITemperatureWidget::OnTemperatureUpdate(const double newTemperature) {
	_updateThermometer(newTemperature);
}

void UAITemperatureWidget::OnHealthUpdate(const double newHealth) {
	_currentHealth = newHealth;
	_updateHealth();
}

void UAITemperatureWidget::OnMaxHealthUpdate(const double newMaxHealth) {
	_maxHealth = newMaxHealth;
	_updateHealth();
}

void UAITemperatureWidget::OnManaUpdate(const double newMana) {
	_currentMana = newMana;
	_updateMana();
}

void UAITemperatureWidget::OnMaxManaUpdate(const double newMaxMana) {
	_maxMana = newMaxMana;
	_updateMana();
}
