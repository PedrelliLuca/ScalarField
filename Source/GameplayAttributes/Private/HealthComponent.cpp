// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"

UHealthComponent::UHealthComponent() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    const float healthRegenPerFrame = _healthRegenPerSecond * deltaTime;
    SetCurrentHealth(_currentHealth + healthRegenPerFrame);
}

#if WITH_EDITOR
void UHealthComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
    FProperty* const property = propertyChangedEvent.Property;
    FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
    if (propertyName == GET_MEMBER_NAME_CHECKED(UHealthComponent, _maxHealth)) {
        if (const auto initTempProperty = CastFieldChecked<FFloatProperty>(property)) {
            SetMaxHealth(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<float>(this)));
        }
    }

    Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UHealthComponent::SetCurrentHealth(const float health) {
    _currentHealth = FMath::Clamp(health, 0., _maxHealth);
    _onHealthChanged.Broadcast(_currentHealth);
    if (FMath::IsNearlyZero(_currentHealth)) {
        SetHealthRegen(0.0f);
        _onDeath.Broadcast(GetOwner());
        SetComponentTickEnabled(false);
    }
}

void UHealthComponent::SetMaxHealth(float maxHealth, const bool bUpdateHealth /*= true*/) {
    maxHealth = FMath::Clamp(maxHealth, 0., TNumericLimits<float>::Max());

    _maxHealth = maxHealth;
    if (bUpdateHealth) {
        SetCurrentHealth(_maxHealth);
    }

    _onMaxHealthChanged.Broadcast(_maxHealth);
}

void UHealthComponent::SetHealthRegen(float healthRegenPerSecond) {
    _healthRegenPerSecond = healthRegenPerSecond;
    _onHealthRegenChanged.Broadcast(_healthRegenPerSecond);
}

void UHealthComponent::BeginPlay() {
    Super::BeginPlay();
    _currentHealth = _maxHealth;
}
