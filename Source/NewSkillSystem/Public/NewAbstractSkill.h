// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ManaComponent.h"
#include "MovementCommandMode.h"
#include "MovementCommandSetter.h"
#include "SkillCastCondition.h"
#include "SkillCastResult.h"
#include "SkillChannelingResult.h"
#include "SkillTargetingCondition.h"
#include "SkillTargetingResult.h"
#include "TacticalPauseWorldSubsystem.h"
#include "UObject/WeakInterfacePtr.h"

#include "NewAbstractSkill.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCastPhaseFinish, FSkillCastResult);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChannelingPhaseFinish, FSkillChannelingResult);

/** Represents a skill of the ScalarField game. */
UCLASS(NotBlueprintable, Abstract)
class NEWSKILLSYSTEM_API UNewAbstractSkill : public UObject, public FTickableGameObject {
    GENERATED_BODY()

public:
    /** \brief Starts the skill casting process. This function can have many outcomes, you can see all of them in ESkillCastResult. Calling this function before
     * SetCaster() will trigger a check. */
    FSkillCastResult TryCast();

    /** \brief Aborts the skill execution: clears the delegates, clears the targets, stops ticking. If shouldResetMovement is set to true, the movement mode
     * will be set to the default one. */
    void Abort(bool shouldResetMovement);

    /** \brief Adds a target to the skill. This function can have many outcomes, you can see all of them in ESkillTargetingResult. Calling this function before
     * SetCaster() will trigger a check. */
    FSkillTargetingResult TryAddTarget(const FSkillTargetPacket& targetPacket);

    void SetCaster(TObjectPtr<AActor> caster);

#pragma region FTickableGameObject

    void Tick(float deltaTime) override;

    /** \brief Determines whether this skill is ready to tick or not by checking whether the skill's caster is valid or not. */
    bool IsTickable() const override;

    /** \brief Determines whether this skill can tick or not. Skills are allowed to tick during their cast and channeling phases. */
    bool IsAllowedToTick() const override;

    /** \brief We have to override this because it's virtual pure in FTickableGameObject, but in practice it's not needed. */
    TStatId GetStatId() const override { return TStatId{}; }

#pragma endregion

    /** \brief Returns the delegate broadcasting what's going on during the casting phase tick. */
    FOnCastPhaseFinish& OnCastPhaseEnd() { return _onCastPhaseEnd; }

    /** \brief Returns the delegate broadcasting what's going on during the channeling phase tick. */
    FOnChannelingPhaseFinish& OnChannelingPhaseEnd() { return _onChannelingPhaseEnd; }

protected:
    const TWeakObjectPtr<AActor>& _getCaster() const { return _caster; }
    float _getChannelingSeconds() const { return _channelingSeconds; }

    /** \brief The time it takes for the skill to be castable again. The countdown starts from the moment the cast is complete. */
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0"))
    float _cooldownSeconds = 1.0f;

    /** \brief Duration of the cast phase of the skill. */
    UPROPERTY(EditDefaultsOnly, Category = "Casting", meta = (ClampMin = "0.0"))
    float _castSeconds = 1.0f;
    /** \brief The cost in mana that is spent during casting. */
    UPROPERTY(EditDefaultsOnly, Category = "Casting", meta = (ClampMin = "0.0"))
    float _castManaCost = 50.0f;

    /** \brief Duration of the channeling phase of the skill. */
    UPROPERTY(EditDefaultsOnly, Category = "Casting", meta = (ClampMin = "0.0"))
    float _channelingSeconds = 1.0f;
    /** \brief The cost in mana that is spent during channeling. */
    UPROPERTY(EditDefaultsOnly, Category = "Casting", meta = (ClampMin = "0.0"))
    float _channelingManaCost = 50.0f;

    // UPROPERTY(EditAnywhere, Category = "Movement Modes")
    // EMovementCommandMode _targetingMovementMode; // TODO?: Could be added in the future, seems excessive for the time being.
    UPROPERTY(EditDefaultsOnly, Category = "Movement Modes")
    EMovementCommandMode _castMovementMode;
    UPROPERTY(EditDefaultsOnly, Category = "Movement Modes")
    EMovementCommandMode _channelingMovementMode;

private:
    /** \brief Represents the concrete cast of the skill, skill-specific logic for the cast is executed in here. */
    virtual void _skillCast() PURE_VIRTUAL(UNewAbstractSkill::_skillCast, return;);
    /** \brief Represents the concrete channeling of the skill, skill-specific logic for channeling ticks is executed in here. */
    virtual void _skillChannelingTick(float deltaTime) PURE_VIRTUAL(UNewAbstractSkill::_channelingTickLogic, return;);
    /** \brief Represents the concrete abortion of the skill, skill-specific logic for cleanup is executed in here. */
    virtual void _skillAbort() PURE_VIRTUAL(UNewAbstractSkill::_channelingTickLogic, return;);

    void _castTick(float deltaTime);
    void _channelingTick(float deltaTime);

    /** \brief Function to call whenever the cast is over. Makes _onCastPhaseEnd broadcast, and executes some extra logic depend on whether the skill requires
     * channeling or not.
     * - Channeling required: movement mode is set to channeling, tick is started if the skill has no cast time.
     * - Channeling not required: Abort() is called. */
    FSkillCastResult _endCast();

    bool _areCastConditionsVerified() const;

    void _onCooldownEnded();

    enum class EMovementModeToSet : uint8
    {
        Default,
        Cast,
        Channeling,
    };
    void _setMovementModeIfPossible(EMovementModeToSet movementModeToSet) const;

    /** \brief Conditions that must be verified by the caster to cast the skill. */
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Conditions")
    TArray<TObjectPtr<USkillCastCondition>> _castConditions{};

    /** \brief Conditions that must be verified by each target required by this skill. */
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Conditions")
    TArray<TObjectPtr<USkillTargetingCondition>> _targetingConditions{};

    /** \brief Kind of target needed by the skill to execute its logic. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting", meta = (MustImplement = "SkillTarget"))
    TSubclassOf<UObject> _targetKind = nullptr;

    /** \brief Num of targets the skill needs to execute its logic. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting")
    int32 _nTargets = 0;

    UPROPERTY()
    TArray<TScriptInterface<ISkillTarget>> _targets{};

    TWeakObjectPtr<AActor> _caster = nullptr;
    TWeakObjectPtr<UManaComponent> _casterManaC = nullptr;
    TWeakInterfacePtr<IMovementCommandSetter> _casterMovementSetterC = nullptr;

    FTimerHandle _cooldownTimer{};
    bool _onCooldown = false;

    bool _isTickAllowed = false;

    float _elapsedExecutionSeconds = 0.0f;
    float _castManaLeftToPay = 0.0f;

    FOnCastPhaseFinish _onCastPhaseEnd{};
    FOnChannelingPhaseFinish _onChannelingPhaseEnd{};

    TWeakObjectPtr<UTacticalPauseWorldSubsystem> _pauseSubSys = nullptr;
};
