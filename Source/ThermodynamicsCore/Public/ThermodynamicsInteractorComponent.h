// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ThermodynamicsInteractorComponent.generated.h"

/**
 * \brief TODO
 */
UCLASS(ClassGroup = (Thermodynamics), meta = (BlueprintSpawnableComponent))
class THERMODYNAMICSCORE_API UThermodynamicsInteractorComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UThermodynamicsInteractorComponent();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

private:
    UPROPERTY(EditAnywhere, Category = "Interaction Range")
    float _interactionRange;
};
