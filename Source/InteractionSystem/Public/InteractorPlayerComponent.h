// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "InteractorInterface.h"

#include "InteractorPlayerComponent.generated.h"

/**
 * \brief Stores all the data related to what the player is currently interacting with.
 */
USTRUCT()
struct FPlayerInteractionData {
    GENERATED_BODY()

    TWeakObjectPtr<UInteractableComponent> InteractableBeingFocused = nullptr;
    TWeakObjectPtr<UInteractableComponent> InteractableBeingInteracted = nullptr;
    double TimestampOfLastFocusCheck = 0.0;
};

UCLASS(ClassGroup = (Interaction), meta = (BlueprintSpawnableComponent))
class INTERACTIONSYSTEM_API UInteractorPlayerComponent : public UActorComponent, public IInteractor {
    GENERATED_BODY()

public:
    UInteractorPlayerComponent();

    // Focus functions
    void PerformFocusCheck() override;

    // Interaction functions
    bool PerformInteractionCheck() override;
    UFUNCTION(BlueprintCallable)
    void EndInteraction() override;
    bool IsInteracting() const override;
    double GetTimeLeftBeforeInteraction() const override;

protected:
    void BeginPlay() override;

private:
    // Focus functions
    void _endFocus() override;
    const TWeakObjectPtr<UInteractableComponent>& _getInteractableBeingFocused() const { return _interactionData.InteractableBeingFocused; }

    // Interaction functions
    void _interact() override;
    const TWeakObjectPtr<UInteractableComponent>& _getInteractableBeingInteracted() const { return _interactionData.InteractableBeingInteracted; }

    TWeakObjectPtr<APlayerController> _ownerPlayerController = nullptr;
    FPlayerInteractionData _interactionData{};

    static constexpr double INTERACTION_TRACE_LENGTH = 100000.0;
    FTimerHandle _interactionTimerHandle{};

    UPROPERTY(EditDefaultsOnly, Category = "Interaction")
    double _timeBetweenFocusChecks = 0.1;
};
