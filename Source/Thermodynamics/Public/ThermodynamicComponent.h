// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ThermodynamicComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTemperatureChanged, double);

/**
 *
 */
UCLASS(ClassGroup=(Thermodynamics), meta = (BlueprintSpawnableComponent))
class THERMODYNAMICS_API UThermodynamicComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UThermodynamicComponent(const FObjectInitializer& ObjectInitializer);

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

	void SetTemperature(double temperature, const bool updateInitialTemp = false);
	double GetTemperature() const { return _currentTemperature; }

	void SetHeatCapacity(double heatCapacity) { _heatCapacity = heatCapacity; }

	void SetCollision(TObjectPtr<UPrimitiveComponent> thermoCollision, TObjectPtr<UPrimitiveComponent> complexCollision = nullptr);

	FOnTemperatureChanged OnTemperatureChanged;

protected:
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Thermodynamic Parameters", meta = (ClampMin = "1"))
	double _heatCapacity = 4184.;

	UPROPERTY(EditAnywhere, Category = "Thermodynamic Parameters")
	double _initialTemperature = 298.15;

private:
	void _setHeatCapacity(double heatCapacity) {
		_heatCapacity = FMath::Clamp(heatCapacity, 1., TNumericLimits<double>::Max());
	}

	double _getTemperatureDelta(float deltaTime);
	void _updateCounterOfChecksThisFrame();
	void _setCurrentTempAsNext();

	void _setInitialExchangers();

	/**
	 * The following two functions specifically mention "Thermodynamics" in their names since SetThermodynamicCollision(), the function that sets up the _thermodynamicCollisionC, requires the input component 
	 * to be a "HeatExchanger". A "HeatExchanger" is a "Thermodynamics" object channel that can only overlap with object that are also "Thermodynamics", so we have the warranty here that overlappedComponent
	 * is in the "Thermodynamics" channel.
	 */
	UFUNCTION()
	void _onSimpleBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
	void _onSimpleEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	TWeakObjectPtr<UPrimitiveComponent> _getMostComplexCollision();

	UPROPERTY(VisibleAnywhere, Category = "Thermodynamic Parameters")
	double _currentTemperature;

	double _nextTemperature;

	// This is the factor (k*A)/L in the heat exchange formulas (4) and (5)
	static constexpr double ROD_CONSTANT = 1.;

	// The number of times this component will be checked this frame by other thermodynamic components
	uint32 _timesToBeCheckedThisFrame = TNumericLimits<uint32>::Max();
	// Count of how many times this component got checked by other thermodynamic component this frame. When the counter reaches _timesToBeCheckedThisFrame, we know for sure we won't be checked again,
	// meaning that we can finally set _currentTemperature = _nextTemperature
	uint32 _counterOfChecksThisFrame = 0;

	TWeakObjectPtr<UPrimitiveComponent> _simpleCollisionC = nullptr;
	TWeakObjectPtr<UPrimitiveComponent> _complexCollisionC = nullptr;

	/** 
	 * Thermodynamic components whose simple collision is colliding with this component's simple collision. We don't know for sure if we can exchange heat with them though, we need to check the relation
	 * between their most complex collisions. That occurs at each tick.
	 */
	TSet<TWeakObjectPtr<UThermodynamicComponent>> _possibleHeatExchangers{};

	bool _bCollisionChangedSinceLastTick = false;
};
