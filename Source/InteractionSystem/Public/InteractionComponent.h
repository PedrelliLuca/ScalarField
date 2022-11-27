// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteraction, TWeakObjectPtr<AController>, interactingCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteraction, TWeakObjectPtr<AController>, interactingCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteraction, TWeakObjectPtr<AController>, interactingCharacter);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractionComponent : public UWidgetComponent {
     GENERATED_BODY()
     
public:
     UInteractionComponent();

     void BeginInteraction(const TWeakObjectPtr<AController> interactingCharacter);
     void Interact(TWeakObjectPtr<AController> interactingCharacter);
     void EndInteraction(const TWeakObjectPtr<AController> interactingCharacter);

     double GetInteractionTime() const { return _interactionTime; }
     double GetInteractionDistance() const { return _interactionDistance; }

protected:
     void BeginPlay() override;
     
     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
     double _interactionTime;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
     double _interactionDistance;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
     FText _interactableNameText;

     UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
     FText _interactableActionText;

     // Broadcasts when the interacting controller starts interacting with the owner actor, i.e. when the interaction key is pressed
     UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
     FOnBeginInteraction _onBeginInteraction;

     // Broadcasts when the interacting controller has interacted with the owner actor for the required amount of time
     UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
     FOnInteraction _onInteraction;

     // Broadcasts when the interacting controller stops interacting with the owner actor, i.e. when the interaction key is released
     UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
     FOnEndInteraction _onEndInteraction;
};
