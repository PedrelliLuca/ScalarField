// Fill out your copyright notice in the Description page of Project Settings.


#include "ManaComponent.h"

UManaComponent::UManaComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UManaComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	const double _manaRegenPerFrame = _manaRegenPerSecond * deltaTime;
	_mana = FMath::Clamp(_mana + _manaRegenPerFrame, 0., _maxMana);
}

#if WITH_EDITOR
void UManaComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
	FProperty* const property = propertyChangedEvent.Property;
	FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(UManaComponent, _maxMana)) {
		if (const auto initTempProperty = CastFieldChecked<FDoubleProperty>(property)) {
			SetMaxMana(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<double>(this)));
		}
	}

	Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UManaComponent::SetMana(const double mana) {
	check(mana >= 0.);
	_mana = mana;
}

void UManaComponent::SetMaxMana(double maxMana, const bool bUpdateMana /*= true*/) {
	maxMana = FMath::Clamp(maxMana, 0., TNumericLimits<double>::Max());

	_maxMana = maxMana;
	if (bUpdateMana) {
		SetMana(_maxMana);
	}
}
