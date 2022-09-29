// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicComponent.h"

UThermodynamicComponent::UThermodynamicComponent() {
}

void UThermodynamicComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
	Super::TickComponent(deltaTime, tickType, thisTickFunction);
}
