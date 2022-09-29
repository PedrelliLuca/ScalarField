// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicComponent.h"

UThermodynamicComponent::UThermodynamicComponent() {
	SetCollisionProfileName("HeatExchanger");
}

void UThermodynamicComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	TArray<TObjectPtr<UPrimitiveComponent>> overlappingComponents;
	GetOverlappingComponents(overlappingComponents);

	for (const auto& comp : overlappingComponents) {
		const TObjectPtr<UThermodynamicComponent> termoComp = Cast<UThermodynamicComponent>(comp);
		check(termoComp != nullptr);
	}
}
