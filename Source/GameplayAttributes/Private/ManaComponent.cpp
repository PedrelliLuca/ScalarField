// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaComponent.h"

UManaComponent::UManaComponent() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UManaComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    const float manaRegenPerFrame = _manaRegenPerSecond * deltaTime;
    SetCurrentMana(_currentMana + manaRegenPerFrame);
}

#if WITH_EDITOR
void UManaComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
    FProperty* const property = propertyChangedEvent.Property;
    FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
    if (propertyName == GET_MEMBER_NAME_CHECKED(UManaComponent, _maxMana)) {
        if (const auto initTempProperty = CastFieldChecked<FFloatProperty>(property)) {
            SetMaxMana(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<float>(this)));
        }
    }

    Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UManaComponent::SetCurrentMana(const float mana) {
    _currentMana = FMath::Clamp(mana, 0., _maxMana);
    _onManaChanged.Broadcast(_currentMana);
}

void UManaComponent::SetMaxMana(float maxMana, const bool bUpdateMana /*= true*/) {
    maxMana = FMath::Clamp(maxMana, 0., TNumericLimits<float>::Max());

    _maxMana = maxMana;
    if (bUpdateMana) {
        SetCurrentMana(_maxMana);
    }

    _onMaxManaChanged.Broadcast(_maxMana);
}

void UManaComponent::SetManaRegen(float manaRegenPerSecond) {
    _manaRegenPerSecond = manaRegenPerSecond;
    _onManaRegenChanged.Broadcast(_manaRegenPerSecond);
}

void UManaComponent::BeginPlay() {
    Super::BeginPlay();
    _currentMana = _maxMana;
}
