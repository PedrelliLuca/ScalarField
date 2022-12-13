// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "StateComponent.generated.h"

UCLASS()
class SKILLUSERFSM_API UStateComponent : public UActorComponent {
     GENERATED_BODY()
     
public:
    bool IsCurrentStateAffectedByPause() const;
    bool IsLookingAtWidget() const;

    void PerformTargetingBehavior(TObjectPtr<AActor> target);
    void PerformSkillExecutionBehavior(int32 skillKey);
    void PerformInteractionBehavior();
    void PerformInventoryToggleBehavior();
    void PerformTickBehavior(float deltaTime);
    void PerformAbortBehavior();


protected:
    void BeginPlay() override;

private:
    // Performs the transition to newState if the state in input is not nullptr
    bool _performStateTransitionRoutine(TObjectPtr<class USkillUserState> newState);

    UPROPERTY()
	TObjectPtr<USkillUserState> _state = nullptr;

    TWeakObjectPtr<AController> _ownerController = nullptr;
};
