// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "InteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractionComponent : public UWidgetComponent {
     GENERATED_BODY()
     
public:
     UInteractionComponent();

     double GetInteractionDistance() const { return _interactionDistance; }

protected:
     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
     double _interactionTime;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
     double _interactionDistance;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
     FText _interactableNameText;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
     FText _interactableActionText;
};
