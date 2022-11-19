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

	// We'll get checked for each potential heat exchanger.
	_timesToBeCheckedThisFrame = _possibleHeatExchangers.Num();

	_nextTemperature = _currentTemperature + _getTemperatureDelta(deltaTime);

	if (_counterOfChecksThisFrame == _timesToBeCheckedThisFrame) {
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

void UNewThermodynamicComponent::SetCollision(TObjectPtr<UPrimitiveComponent> simpleCollision, TObjectPtr<UPrimitiveComponent> complexCollision /*= nullptr*/) {
	check(IsValid(simpleCollision));

	if (_simpleCollisionC.IsValid()) {
		_simpleCollisionC->OnComponentBeginOverlap.RemoveDynamic(this, &UNewThermodynamicComponent::_onSimpleBeginOverlap);
		_simpleCollisionC->OnComponentEndOverlap.RemoveDynamic(this, &UNewThermodynamicComponent::_onSimpleEndOverlap);

		_counterOfChecksThisFrame = 0;
		_timesToBeCheckedThisFrame = TNumericLimits<uint32>::Max();
		_possibleHeatExchangers.Empty();
	}

	// Is the input collision an actual thermodynamic collider?
	check(simpleCollision->GetCollisionProfileName() == TEXT("HeatExchanger"));

	_simpleCollisionC = simpleCollision;
	_simpleCollisionC->OnComponentBeginOverlap.AddDynamic(this, &UNewThermodynamicComponent::_onSimpleBeginOverlap);
	_simpleCollisionC->OnComponentEndOverlap.AddDynamic(this, &UNewThermodynamicComponent::_onSimpleEndOverlap);

	_complexCollisionC = nullptr;
	if (IsValid(complexCollision)) {
		// Is the input collision an actual thermodynamic collider?
		check(complexCollision->GetCollisionProfileName() == TEXT("HeatExchanger"));
		_complexCollisionC = complexCollision;
	}

	_bCollisionChangedSinceLastTick = true;
}

void UNewThermodynamicComponent::BeginPlay() {
	Super::BeginPlay();
	SetTemperature(_initialTemperature);
}

double UNewThermodynamicComponent::_getTemperatureDelta(float deltaTime) {
	double deltaTemperature = 0.0;
	if (_possibleHeatExchangers.Num() == 0) {
		return deltaTemperature;
	}

	const auto thisCollision = _getMostComplexCollision();

	// Here this component performs the heat-checks on the other components.
	for (const auto& otherThermoC : _possibleHeatExchangers) {
		check(otherThermoC.IsValid());
		const auto otherCollison = otherThermoC->_getMostComplexCollision();

		// If the following evaluates to true, that means that otherThermoC is an actual heatExchanger for thisThermoC. 
		if (thisCollision->IsOverlappingComponent(otherCollison.Get())) {
			/* When this is hotter than other, the delta is negative since we emit heat.
			 * When this is colder than other, the delta is positive since we absorb heat. */
			deltaTemperature += (otherThermoC->_currentTemperature - _currentTemperature);
		}

		// We heat-checked otherThermoC, so it must increase its counter
		otherThermoC->_updateCounterOfChecksThisFrame();
	}

	deltaTemperature /= _possibleHeatExchangers.Num();
	deltaTemperature *= ROD_CONSTANT * deltaTime / _heatCapacity;
	return deltaTemperature;
}

void UNewThermodynamicComponent::_updateCounterOfChecksThisFrame() {
	++_counterOfChecksThisFrame;
	if (_counterOfChecksThisFrame == _timesToBeCheckedThisFrame) {
		_setCurrentTempAsNext();
	}
}

void UNewThermodynamicComponent::_setCurrentTempAsNext() {
	_currentTemperature = _nextTemperature;
	OnTemperatureChanged.Broadcast(_currentTemperature);

	_counterOfChecksThisFrame = 0;
	_timesToBeCheckedThisFrame = TNumericLimits<uint32>::Max();
}

void UNewThermodynamicComponent::_setInitialExchangers() {
	check(_simpleCollisionC.IsValid());

	TArray<TObjectPtr<UPrimitiveComponent>> overlappingComponents;
	_simpleCollisionC->GetOverlappingComponents(overlappingComponents);

	for (const auto& otherC : overlappingComponents) {
		const auto otherOwner = otherC->GetOwner();

		const auto otherThermoC = otherOwner->FindComponentByClass<UNewThermodynamicComponent>();
		// If I have a thermodynamic collision I must have a thermodynamic component
		check(IsValid(otherThermoC));

		// Filtering out every collision that's not simple VS simple
		if (otherThermoC->_simpleCollisionC.Get() != otherC) {
			check(otherThermoC->_complexCollisionC.Get() == otherC);
			continue;
		}

		_possibleHeatExchangers.Emplace(otherThermoC);
	}
}

void UNewThermodynamicComponent::_onSimpleBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
	const auto otherThermoC = otherActor->FindComponentByClass<UNewThermodynamicComponent>();
	check(IsValid(otherThermoC));

	// Filtering out every collision that's not simple VS simple
	if (otherThermoC->_simpleCollisionC.Get() != otherComp) {
		// NOTE: THIS WHOLE SYSTEM WORKS AS LONG AS YOU HAVE A SINGLE THERMODYNAMIC COMPONENT FOR EACH ACTOR. Only if this hypothesys holds you can be sure otherComp is either the otherThermoC's simple or complex collision.
		check(otherThermoC->_complexCollisionC.Get() == otherComp);
		return;
	}

	_possibleHeatExchangers.Emplace(otherThermoC);
}

void UNewThermodynamicComponent::_onSimpleEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
	const auto thermoCToRemove = Algo::FindByPredicate(_possibleHeatExchangers, [&otherComp](const TWeakObjectPtr<UNewThermodynamicComponent>& thermoC) {
		return thermoC->_simpleCollisionC.Get() == otherComp;
	});

	// thermoCToRemove could be nullptr, meaning that otherComp is a complex collision
	if (thermoCToRemove != nullptr) {
		_possibleHeatExchangers.Remove(*thermoCToRemove);
	}
}

TWeakObjectPtr<UPrimitiveComponent> UNewThermodynamicComponent::_getMostComplexCollision() {
	if (_complexCollisionC.IsValid()) {
		return _complexCollisionC;
	}

	return _simpleCollisionC;
}
