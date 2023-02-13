// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AITemperatureWidget.generated.h"

/**
 * 
 */
UCLASS()
class SCALARFIELD_API UAITemperatureWidget : public UUserWidget {
	GENERATED_BODY()

public:
	void SetComfortIntervalLimits(float upperLimit, float lowerLimit);

	void OnTemperatureUpdate(double newTemperature);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void _updateThermometer(double newTemperature);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comfort Interval")
	float _comfortUpperLimit = -1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comfort Interval")
	float _comfortLowerLimit = -1.0f;
	
};
