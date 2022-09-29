// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "ThermodynamicComponent.generated.h"

/**
 * 
 */
UCLASS()
class SCALARFIELD_API UThermodynamicComponent : public UCapsuleComponent
{
	GENERATED_BODY()
	
public:
	UThermodynamicComponent();

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	double GetTemperature() const { return _currentTemperature; }
	void SetTemperature(double temperature) {
		_currentTemperature = temperature;
		_nextTemperature = temperature;
	}

private:
	double _currentTemperature = 298.15;
	double _nextTemperature = 298.15;
	double _heatCapacity = 4184.;
	static constexpr double ROD_CONSTANT = 1.;

	uint32 _numOfInteractors = TNumericLimits<uint32>::Max();
	uint32 _counterOfInteractors = 0;

};
