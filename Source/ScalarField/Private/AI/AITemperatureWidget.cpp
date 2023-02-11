// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AITemperatureWidget.h"

void UAITemperatureWidget::SetComfortIntervalLimits(const float upperLimit, const float lowerLimit) {
	_comfortUpperLimit = upperLimit;
	_comfortLowerLimit = lowerLimit;
}

void UAITemperatureWidget::OnTemperatureUpdate(const double newTemperature) {
	_updateThermometer(newTemperature);
}
