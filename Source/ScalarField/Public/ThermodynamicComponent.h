// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "ThermodynamicComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup=(Thermodynamics), meta = (BlueprintSpawnableComponent))
class SCALARFIELD_API UThermodynamicComponent : public UCapsuleComponent
{
	GENERATED_BODY()

public:
	UThermodynamicComponent(const FObjectInitializer& ObjectInitializer);

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	double GetTemperature() const { return _currentTemperature; }

protected:
	UPROPERTY(EditAnywhere, Category = "Thermodynamics")
	double _heatCapacity = 4184.;

	UPROPERTY(EditAnywhere, Category = "Thermodynamics")
	double _initialTemperature = 298.15;

private:
	void _setTemperature(double temperature) {
		// Updating _initialTemperature just to give a visual feedback in the editor, this value doesn't matter during play
		_initialTemperature = FMath::Clamp(temperature, 0., TNumericLimits<double>::Max());

		_currentTemperature = _initialTemperature;
		_nextTemperature = _initialTemperature;
	}
	void _setHeatCapacity(double heatCapacity) {
		_heatCapacity = FMath::Clamp(heatCapacity, 1., TNumericLimits<double>::Max());
	}

	double _getTemperatureDelta(const TArray<TObjectPtr<UPrimitiveComponent>>& overlappingComponents, float deltaTime);
	void _increaseInteractorsCount();
	void _setCurrentTempAsNext();

	UPROPERTY(VisibleAnywhere, Category = "Thermodynamics")
	double _currentTemperature;

	double _nextTemperature;

	// This is the factor (k*A)/L in the heat exchange formulas (4) and (5)
	static constexpr double ROD_CONSTANT = 1.;

	uint32 _numOfInteractors = TNumericLimits<uint32>::Max();
	uint32 _counterOfInteractors = 0;
};
