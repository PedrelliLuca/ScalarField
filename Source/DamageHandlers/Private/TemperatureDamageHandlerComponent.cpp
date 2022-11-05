// Fill out your copyright notice in the Description page of Project Settings.


#include "TemperatureDamageHandlerComponent.h"
#include "TemperatureDamageType.h"

UTemperatureDamageHandlerComponent::UTemperatureDamageHandlerComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTemperatureDamageHandlerComponent::HandleDamage(const double temperature) {

	if (IsComponentTickEnabled()) {
		// Damage handler on cooldown
		return;
	}

	if (IsTemperatureComfortable(temperature)) {
		// This temperature doesn't cause any damage to the owner
		return;
	}

	// The temperature harms the owner
	const TSubclassOf<UDamageType> damageType = UTemperatureDamageType::StaticClass();
	FDamageEvent damageEvent{ damageType };
	GetOwner()->TakeDamage(_computeDamageFromTemperature(temperature), damageEvent, nullptr, GetOwner());

	SetComponentTickEnabled(true);
}

void UTemperatureDamageHandlerComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	// TickComponent() manages the cooldown of the temperature damage deltaTime

	_currentInteralTime += deltaTime;
	if (_currentInteralTime > _damageInterval) {
		_currentInteralTime = 0.;
		SetComponentTickEnabled(false);
	}
}

void UTemperatureDamageHandlerComponent::SetMinComfortTemperature(const double minComfortTemperature) {
	_minComfortTemperature = minComfortTemperature > 0. ? minComfortTemperature : 0.;
}

void UTemperatureDamageHandlerComponent::SetMaxComfortTemperature(const double maxComfortTemperature) {
	_maxComfortTemperature = maxComfortTemperature > _minComfortTemperature ? maxComfortTemperature : _minComfortTemperature + DBL_EPSILON;
}

#if WITH_EDITOR
void UTemperatureDamageHandlerComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
	FProperty* const property = propertyChangedEvent.Property;
	FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(UTemperatureDamageHandlerComponent, _minComfortTemperature)) {
		if (const auto initTempProperty = CastFieldChecked<FDoubleProperty>(property)) {
			SetMinComfortTemperature(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<double>(this)));
		}
	}
	else if (propertyName == GET_MEMBER_NAME_CHECKED(UTemperatureDamageHandlerComponent, _maxComfortTemperature)) {
		if (const auto initTempProperty = CastFieldChecked<FDoubleProperty>(property)) {
			SetMaxComfortTemperature(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<double>(this)));
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

double UTemperatureDamageHandlerComponent::_computeDamageFromTemperature(const double temperature) {
	if (temperature < _minComfortTemperature) {
		return _minComfortTemperature - temperature;
	}
	else if (temperature > _maxComfortTemperature) {
		return temperature - _maxComfortTemperature;
	}

	checkNoEntry();
	return 0.;
}