// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbstractSkill.h"
#include "Components/ActorComponent.h"
#include "SkillCastResult.h"
#include "SkillTarget.h"
#include "SkillTargetingResult.h"

#include "SkillsContainerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillExecutionStatusChanged, bool);

UCLASS(ClassGroup = (Skills), meta = (BlueprintSpawnableComponent))
class SKILLSYSTEM_API USkillsContainerComponent : public UActorComponent {
    GENERATED_BODY()

    friend class FSkillsContainerInspector;

public:
    USkillsContainerComponent();

    /** \brief Given the index of a skill stored within this component, this function tries to cast it. */
    FSkillCastResult TryCastSkillAtIndex(int32 index);

    /** \brief Tries to cast the waiting skill, if any. In case there is no waiting skill, an invalid TOptional is returned. */
    TOptional<FSkillCastResult> TryCastWaitingSkill();

    /** \brief If a skill is being executed, its execution is aborted and true is returned. If no skill is being executed, nothing occurs and false is returned.
     */
    bool AbortSkillInExecution();

    /** \brief If a skill is waiting for targets, this makes it forget its targets and true is returned. If no skill is waiting, nothing occurs and false is
     * returned. */
    bool AbortWaitingSkill();

    /** \brief If a skill is waiting for targets, tries adding it and returns the result of the operation. If no skill is waiting for targets, the returned
     * TOptional is unset. */
    TOptional<FSkillTargetingResult> TryAddTargetToWaitingSkill(const FSkillTargetPacket& targetPacket);

    FOnSkillExecutionStatusChanged& OnSkillInExecutionStatusChanged() { return _onSkillInExecutionStatusChanged; }

protected:
    void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Skills")
    TArray<TSubclassOf<UAbstractSkill>> _skillClasses{};

private:
    void _setNewSkillInExecution(TObjectPtr<UAbstractSkill> skill, ESkillCastResult castResultValue);
    void _onCurrentlyExecutedSkillCastPhaseEnd(FSkillCastResult skillCastResult);
    void _onCurrentlyExecutedSkillChannelingPhaseEnd(FSkillChannelingResult skillChannelingResult);

    bool _resetSkillInExecution(bool resetMovement);
    bool _resetWaitingSkill();

    UPROPERTY()
    TArray<TObjectPtr<UAbstractSkill>> _skills{};

    TWeakObjectPtr<UAbstractSkill> _skillWaitingForTargets = nullptr;
    TWeakObjectPtr<UAbstractSkill> _currentlyExecutedSkill = nullptr;

    FOnSkillExecutionStatusChanged _onSkillInExecutionStatusChanged;
};
