// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, TWeakObjectPtr<AController>, interactingController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, TWeakObjectPtr<AController>, interactingController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteraction, TWeakObjectPtr<AController>, interactingController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteraction, TWeakObjectPtr<AController>, interactingController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteraction, TWeakObjectPtr<AController>, interactingController);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractionComponent : public UWidgetComponent {
     GENERATED_BODY()
     
public:
     UInteractionComponent();

     void BeginFocus(TWeakObjectPtr<AController> interactingController);
     void EndFocus(TWeakObjectPtr<AController> interactingController);
     void BeginInteraction(TWeakObjectPtr<AController> interactingController);
     void EndInteraction(TWeakObjectPtr<AController> interactingController);
     void Interact(TWeakObjectPtr<AController> interactingController);

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

     // Broadcasts when the interacting controller starts focusing the owner actor (for a player controller, when the
     // cursor is hovering the owner actor)
     UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
     FOnBeginFocus _onBeginFocus;

     // Broadcasts when the interacting controller stops focusing the owner actor (for a player controller, when the
     // cursor stops hovering the owner actor)
     UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
     FOnEndFocus _onEndFocus;

     // Broadcasts when the interacting controller starts interacting with the owner actor (for a player controller,
     // when the interaction key is pressed)
     UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
     FOnBeginInteraction _onBeginInteraction;

     // Broadcasts when the interacting controller has interacted with the owner actor for the required amount of time
     // (for a player controller, when the interaction key has been held down for the required amount of time)
     UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
     FOnInteraction _onInteraction;

     // Broadcasts when the interacting controller stops interacting with the owner actor (for a player controller, when
     // the interaction key is released)
     UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
     FOnEndInteraction _onEndInteraction;
};
