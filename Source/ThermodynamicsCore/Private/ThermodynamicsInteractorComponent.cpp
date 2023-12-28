// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermodynamicsInteractorComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "HeatmapGridOperations.h"

UThermodynamicsInteractorComponent::UThermodynamicsInteractorComponent()
    : _interactionRange(0.0f) {
    PrimaryComponentTick.bCanEverTick = true;
}

void UThermodynamicsInteractorComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    const auto location = FVector2D(GetOwner()->GetActorLocation());
    HeatmapGrid::Interact(location, _interactionRange);
}
