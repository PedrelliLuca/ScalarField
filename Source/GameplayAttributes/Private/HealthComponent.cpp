// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	const double healthRegenPerFrame = _healthRegenPerSecond * deltaTime;
	_health = FMath::Clamp(_health + healthRegenPerFrame, 0., _maxHealth);
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

void UHealthComponent::SetHealth(const double mana) {
	check(mana >= 0.);
	_health = mana;
}

void UHealthComponent::SetMaxHealth(double maxHealth, const bool bUpdateHealth /*= true*/) {
	maxHealth = FMath::Clamp(maxHealth, 0., TNumericLimits<double>::Max());

	_maxHealth = maxHealth;
	if (bUpdateHealth) {
		SetHealth(_maxHealth);
	}
}
