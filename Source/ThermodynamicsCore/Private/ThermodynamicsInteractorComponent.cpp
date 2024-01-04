// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermodynamicsInteractorComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HeatmapGridOperations.h"
#include "ThermodynamicsSubsystem.h"

UThermodynamicsInteractorComponent::UThermodynamicsInteractorComponent()
    : _initialTemperature(273.15f) // 0 Celsius degrees.
    , _heatCapacity(1.0f)
    , _interactionRange(0.0f)
    , _currentTemperature(0.0f)
    , _nextTemperature(0.0f) {
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = ETickingGroup::TG_PrePhysics;
}

float UThermodynamicsInteractorComponent::GetTemperature() const {
    return _currentTemperature;
}

void UThermodynamicsInteractorComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    // 1) Interact with other UThermodynamicsInteractorComponents you're colliding with
    // TODO....
    // const float currDeltaT_OtherBodies = ...;

    // 2) Interact with the Heatmap Grid
    const auto location = FVector2D(GetOwner()->GetActorLocation());
    const float currDeltaT_GridNormalized = HeatmapGrid::Interact(location, _interactionRange, _currentTemperature, deltaTime);

    const float totalCurrDeltaT = currDeltaT_GridNormalized /* + currDeltaT_OtherBodies */;
    const float totalDeltaT = (UThermodynamicsSubsystem::ROD_CONSTANT * totalCurrDeltaT * deltaTime) / _heatCapacity;

    _nextTemperature += totalDeltaT;

    // TODO: Delete this. This is ok only because we're not handling body to body exchanges for the time being! A body can't update its own current temperature
    // because we have no guarantees that the other bodies in the same tick group already interacted with it.
    _currentTemperature = _nextTemperature;
}

#if WITH_EDITOR
void UThermodynamicsInteractorComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
    FProperty* const property = propertyChangedEvent.Property;
    FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
    if (propertyName == GET_MEMBER_NAME_CHECKED(UThermodynamicsInteractorComponent, _initialTemperature)) {
        if (const auto initTempProperty = CastFieldChecked<FFloatProperty>(property)) {
            float initialTemperature = initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<float>(this));
            initialTemperature = FMath::Clamp(initialTemperature, 0., TNumericLimits<float>::Max());

            _initialTemperature = initialTemperature;
            _currentTemperature = initialTemperature;
            _nextTemperature = initialTemperature;
        }
    }

    Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UThermodynamicsInteractorComponent::BeginPlay() {
    Super::BeginPlay();

    _currentTemperature = _initialTemperature;
    _nextTemperature = _initialTemperature;
}
