// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	const double healthRegenPerFrame = _healthRegenPerSecond * deltaTime;
	SetCurrentHealth(_currentHealth + healthRegenPerFrame);
}

#if WITH_EDITOR
void UHealthComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
	FProperty* const property = propertyChangedEvent.Property;
	FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(UHealthComponent, _maxHealth)) {
		if (const auto initTempProperty = CastFieldChecked<FDoubleProperty>(property)) {
			SetMaxHealth(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<double>(this)));
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UHealthComponent::SetCurrentHealth(const double health) {
	_currentHealth =  FMath::Clamp(health, 0., _maxHealth);
	_onHealthChanged.Broadcast(health);
}

void UHealthComponent::SetMaxHealth(double maxHealth, const bool bUpdateHealth /*= true*/) {
	maxHealth = FMath::Clamp(maxHealth, 0., TNumericLimits<double>::Max());

	_maxHealth = maxHealth;
	if (bUpdateHealth) {
		SetCurrentHealth(_maxHealth);
	}

	_onMaxHealthChanged.Broadcast(maxHealth);
}

void UHealthComponent::SetHealthRegen(double healthRegenPerSecond) {
	_healthRegenPerSecond = healthRegenPerSecond;
	_onHealthRegenChanged.Broadcast(healthRegenPerSecond);
}
