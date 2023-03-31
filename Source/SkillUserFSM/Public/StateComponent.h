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

    /**
     * \brief Sets a target for the skill currently in execution. If the target is valid, the execution of the skills continues and true is returned. If the
     * target is invalid or no skill is in execution, false is returned and the current state is kept.
     */
    bool PerformTargetingBehavior(TObjectPtr<AActor> target);

    /**
     * \brief Starts the execution of the skill in input. Returns true if the execution can start, false otherwise. Execution start can fail in many cases. For
     * example:
     * - In case the skill is on cooldown
     * - In case the skill is already being executed
     * - In case the current state does not allow the execution of skills
     */
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
    /**
     * \brief Performs a transition to newState. If newState is nullptr, it means that we should keep the current state, and no transition is performed.
     * Returns true if the transition has been performed successfully (newState != nullptr), false otherwise.
     */
    bool _performStateTransitionRoutine(TObjectPtr<class USkillUserState> newState);

    UPROPERTY()
    TObjectPtr<USkillUserState> _state = nullptr;

    TWeakObjectPtr<AController> _ownerController = nullptr;

    FOnSkillExecutionBegin _onSkillExecutionBegin;
    FOnSkillExecutionEnd _onSkillExecutionEnd;
};
