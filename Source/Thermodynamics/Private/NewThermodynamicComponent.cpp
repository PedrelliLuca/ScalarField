// Fill out your copyright notice in the Description page of Project Settings.


#include "NewThermodynamicComponent.h"

UNewThermodynamicComponent::UNewThermodynamicComponent(const FObjectInitializer& ObjectInitializer) {
	PrimaryComponentTick.bCanEverTick = true;

	_currentTemperature = _initialTemperature;
	_nextTemperature = _initialTemperature;
}

void UNewThermodynamicComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (_bCollisionChangedSinceLastTick) {
		// UPrimitiveComponent::OnComponentBeginOverlap does not fire before the first tick. We need this call to get the collisions we're already
		// overlapping with.
		_setInitialExchangers();
		_bCollisionChangedSinceLastTick = false;
	}

	_heatExchangesToPerformThisFrame = _heatExchangers.Num();
	_nextTemperature = _currentTemperature + _getTemperatureDelta(deltaTime);

	if (_heatExchangesOccurredThisFrame == _heatExchangesToPerformThisFrame) {
		_setCurrentTempAsNext();
	}
}

#if WITH_EDITOR
void UNewThermodynamicComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
	FProperty* const property = propertyChangedEvent.Property;
	FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(UNewThermodynamicComponent, _initialTemperature)) {
		if (const auto initTempProperty = CastFieldChecked<FDoubleProperty>(property)) {
			SetTemperature(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<double>(this)), true);
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UNewThermodynamicComponent::SetTemperature(double temperature, const bool updateInitialTemp /*= false*/) {
	temperature = FMath::Clamp(temperature, 0., TNumericLimits<double>::Max());

	if (updateInitialTemp) {
		// Updating _initialTemperature just to give a visual feedback in the editor, this value doesn't matter during play
		_initialTemperature = temperature;
	}

	_currentTemperature = temperature;
	_nextTemperature = temperature;
}

void UNewThermodynamicComponent::SetThermodynamicCollision(TObjectPtr<UPrimitiveComponent> thermoCollision) {
	check(IsValid(thermoCollision));

	// SetThermodynamicCollision() can be called just once
	check(!_thermodynamicCollisionC.IsValid());

	// Is the input collision an actual thermodynamic collider?
	const auto profile = thermoCollision->GetCollisionProfileName();
	check(profile == TEXT("HeatExchanger"));

	_thermodynamicCollisionC = thermoCollision;
	_thermodynamicCollisionC->OnComponentBeginOverlap.AddDynamic(this, &UNewThermodynamicComponent::_onThermodynamicOverlapBegin);
	_thermodynamicCollisionC->OnComponentEndOverlap.AddDynamic(this, &UNewThermodynamicComponent::_onThermodynamicOverlapEnd);

	_bCollisionChangedSinceLastTick = true;
}

void UNewThermodynamicComponent::BeginPlay() {
	Super::BeginPlay();
	SetTemperature(_initialTemperature);
}

double UNewThermodynamicComponent::_getTemperatureDelta(float deltaTime) {
	double deltaTemperature = 0.0;
	if (_heatExchangers.Num() == 0) {
		return deltaTemperature;
	}

	for (const auto& otherThermoC : _heatExchangers) {
		check(otherThermoC.IsValid());

		/* When this is hotter than other, the delta is negative since we emit heat
		 * When this is colder than other, the delta is positive since we absorb heat. */
		deltaTemperature += (otherThermoC->_currentTemperature - _currentTemperature);

		otherThermoC->_increaseOccurredHeatExchangesCount();
	}

	deltaTemperature /= _heatExchangers.Num();
	deltaTemperature *= ROD_CONSTANT * deltaTime / _heatCapacity;
	return deltaTemperature;
}

void UNewThermodynamicComponent::_increaseOccurredHeatExchangesCount() {
	++_heatExchangesOccurredThisFrame;
	if (_heatExchangesOccurredThisFrame == _heatExchangesToPerformThisFrame) {
		_setCurrentTempAsNext();
	}
}

void UNewThermodynamicComponent::_setCurrentTempAsNext() {
	_currentTemperature = _nextTemperature;
	OnTemperatureChanged.Broadcast(_currentTemperature);

	_heatExchangesOccurredThisFrame = 0;
	_heatExchangesToPerformThisFrame = TNumericLimits<uint32>::Max();
}

void UNewThermodynamicComponent::_setInitialExchangers() {
	check(_thermodynamicCollisionC.IsValid());

	TArray<TObjectPtr<UPrimitiveComponent>> overlappingComponents;
	_thermodynamicCollisionC->GetOverlappingComponents(overlappingComponents);

	for (const auto& otherC : overlappingComponents) {
		const auto otherOwner = otherC->GetOwner();
		// HeatExchangers can overlap only with other HeatExchangers, and there can be only one thermodynamic collider per actor.
		// This implies that it has to be is impossible for otherC to have the same component as this.
		check(otherOwner != GetOwner());

		const auto otherThermoC = otherOwner->FindComponentByClass<UNewThermodynamicComponent>();
		// If I have a thermodynamic collision I must have a thermodynamic component
		check(IsValid(otherThermoC));

		_heatExchangers.Emplace(otherThermoC);
	}
}

void UNewThermodynamicComponent::_onThermodynamicOverlapBegin(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
	const auto otherThermoC = otherActor->FindComponentByClass<UNewThermodynamicComponent>();

	// NOTE: THIS WHOLE SYSTEM WORKS AS LONG AS YOU HAVE A SINGLE THERMODYNAMIC COMPONENT FOR EACH ACTOR. Only if this hypothesys holds you can be sure to retrieve the thermodynamic component associated
	// with the overlappedComponent in input.
	check(IsValid(otherThermoC));
	check(otherThermoC->_thermodynamicCollisionC.Get() == otherComp);

	_heatExchangers.Emplace(otherThermoC);
}

void UNewThermodynamicComponent::_onThermodynamicOverlapEnd(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
	const auto thermoCToRemove = Algo::FindByPredicate(_heatExchangers, [&otherComp](const TWeakObjectPtr<UNewThermodynamicComponent>& thermoC) {
		return thermoC->_thermodynamicCollisionC.Get() == otherComp;
	});

	check(thermoCToRemove != nullptr);
	_heatExchangers.Remove(*thermoCToRemove);
}
