// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "Components/ActorComponent.h"

#include "StateComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSkillExecutionBegin);
DECLARE_MULTICAST_DELEGATE(FOnSkillExecutionEnd);

UCLASS()
class SKILLUSERFSM_API UStateComponent : public UActorComponent {
     GENERATED_BODY()
     
public:
    bool IsCurrentStateAffectedByPause() const;

    void PerformTargetingBehavior(TObjectPtr<AActor> target);
    bool PerformSkillExecutionBehavior(TObjectPtr<UAbstractSkill> skill);
    void PerformInteractionBehavior();
    void PerformInventoryToggleBehavior();
    void PerformTickBehavior(float deltaTime);
    void PerformAbortBehavior();

    FOnSkillExecutionBegin& OnSkillExecutionBegin() { return _onSkillExecutionBegin; }
    FOnSkillExecutionEnd& OnSkillExecutionEnd() { return _onSkillExecutionEnd; }

protected:
    void BeginPlay() override;

private:
    // Performs the transition to newState if the state in input is not nullptr
    bool _performStateTransitionRoutine(TObjectPtr<class USkillUserState> newState);

    UPROPERTY()
	TObjectPtr<USkillUserState> _state = nullptr;

    TWeakObjectPtr<AController> _ownerController = nullptr;

    FOnSkillExecutionBegin _onSkillExecutionBegin;
    FOnSkillExecutionEnd _onSkillExecutionEnd;
};
