// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicComponent.h"

UThermodynamicComponent::UThermodynamicComponent(const FObjectInitializer& objectInitializer) : Super(objectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;

	// This capsule only collides on the "Thermodynamics" channel
	SetCollisionProfileName("HeatExchanger");

	_currentTemperature = _initialTemperature;
	_nextTemperature = _initialTemperature;
}

void UThermodynamicComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	TArray<TObjectPtr<UPrimitiveComponent>> overlappingComponents;
	GetOverlappingComponents(overlappingComponents);
	_numOfInteractors = overlappingComponents.Num();

	_nextTemperature = _currentTemperature + _getTemperatureDelta(overlappingComponents, deltaTime);

	if (_counterOfInteractors == _numOfInteractors) {
		_setCurrentTempAsNext();
	}
}

#if WITH_EDITOR
void UThermodynamicComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
	FProperty* const property = propertyChangedEvent.Property;
	FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(UThermodynamicComponent, _initialTemperature)) {
		if (const auto initTempProperty = CastFieldChecked<FDoubleProperty>(property)) {
			SetTemperature(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<double>(this)), true);
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UThermodynamicComponent::SetTemperature(double temperature, const bool updateInitialTemp /*= false*/) {
	// Updating _initialTemperature just to give a visual feedback in the editor, this value doesn't matter during play
	temperature = FMath::Clamp(temperature, 0., TNumericLimits<double>::Max());

	if (updateInitialTemp) {
		_initialTemperature = temperature;
	}

	_currentTemperature = temperature;
	_nextTemperature = temperature;
}


double UThermodynamicComponent::_getTemperatureDelta(const TArray<TObjectPtr<UPrimitiveComponent>>& overlappingComponents, const float deltaTime) {
	double deltaTemperature = 0.;
	if (overlappingComponents.Num() == 0) {
		return deltaTemperature;
	}

	for (const auto& otherC : overlappingComponents) {
		const TObjectPtr<UThermodynamicComponent> otherTermoC = Cast<UThermodynamicComponent>(otherC);
		check(otherTermoC != nullptr);

		/* When this is hotter than other, the delta is negative since we emit heat
		 * When this is colder than other, the delta is positive since we absorb heat. */
		deltaTemperature += (otherTermoC->_currentTemperature - _currentTemperature);

		otherTermoC->_increaseInteractorsCount();
	}

	deltaTemperature /= overlappingComponents.Num();
	deltaTemperature *= ROD_CONSTANT * deltaTime / _heatCapacity;
	return deltaTemperature;
}

void UThermodynamicComponent::_increaseInteractorsCount() {
	++_counterOfInteractors;
	if (_counterOfInteractors == _numOfInteractors) {
		_setCurrentTempAsNext();
	}
}

void UThermodynamicComponent::_setCurrentTempAsNext() {
	_currentTemperature = _nextTemperature;
	OnTemperatureChanged.Broadcast(_currentTemperature);

	_counterOfInteractors = 0;
	_numOfInteractors = TNumericLimits<uint32>::Max();
}
