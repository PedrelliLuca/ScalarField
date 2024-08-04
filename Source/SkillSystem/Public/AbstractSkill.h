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

#include "AbstractSkill.generated.h"

UENUM(BlueprintType)
enum class ESkillStatus : uint8
{
    None UMETA(DisplayName = "Available (None)"),
    Targeting UMETA(DisplayName = "Targeting"),
    Casting UMETA(DisplayName = "Casting"),
    Channeling UMETA(DisplayName = "Channeling"),
    Cooldown UMETA(DisplayName = "Cooldown"),
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillStatusChanged, ESkillStatus);

/** Represents a skill of the ScalarField game. */
UCLASS(NotBlueprintable, Abstract)
class SKILLSYSTEM_API UAbstractSkill : public UObject, public FTickableGameObject {
    GENERATED_BODY()

    friend class FSkillPropertiesInspector;

public:
    /** \brief Starts the skill casting process. This function can have many outcomes, you can see all of them in ESkillCastResult. Calling this function before
     * SetCaster() will trigger a check. */
    FSkillCastResult TryCast();

    /** \brief Aborts the skill execution: clears the delegate, clears the targets, stops ticking. If shouldResetMovement is set to true, the movement mode
     * will be set to the default one. Determines whether the cooldown timer should start or not. */
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

    FOnSkillStatusChanged& OnSkillStatusChanged() { return _onSkillStatusChanged; }

protected:
    const TWeakObjectPtr<AActor>& _getCaster() const { return _caster; }
    const TArray<TScriptInterface<ISkillTarget>>& _getTargets() const { return _targets; }

    float _getChannelingSeconds() const { return _channelingSeconds; }

    /** \brief The time it takes for the skill to be castable again. The countdown starts when one of the following is true:
    1. The cast is complete and there is no channeling.
    2. The channeling is complete.
    3. The channeling is aborted. */
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

    /** \brief If true, targeting conditions are checked while channeling. This implies that, if one of them fails, the skill is aborted. */
    UPROPERTY(EditDefaultsOnly, Category = "Chanelling")
    bool _checkTargetingConditionsWhenChanneling = false;

    UPROPERTY(EditDefaultsOnly, Category = "Movement Modes")
    EMovementCommandMode _castMovementMode;
    UPROPERTY(EditDefaultsOnly, Category = "Movement Modes")
    EMovementCommandMode _channelingMovementMode;

private:
    void _abort(bool shouldResetMovement, bool shouldStartCooldownTimer);

    /** \brief Represents the concrete cast of the skill, skill-specific logic for the cast is executed in here. */
    virtual void _skillCast() PURE_VIRTUAL(UAbstractSkill::_skillCast, return;);
    /** \brief Represents the concrete channeling of the skill, skill-specific logic for channeling ticks is executed in here. */
    virtual void _skillChannelingTick(float deltaTime) PURE_VIRTUAL(UAbstractSkill::_channelingTickLogic, return;);
    /** \brief Represents the concrete abortion of the skill, skill-specific logic for cleanup is executed in here. */
    virtual void _skillAbort() PURE_VIRTUAL(UAbstractSkill::_channelingTickLogic, return;);

    void _castTick(float deltaTime);
    void _channelingTick(float deltaTime);

    /** \brief Function to call whenever the cast is over. Makes _onCastPhaseEnd broadcast, and executes some extra logic depend on whether the skill requires
     * channeling or not.
     * - Channeling required: movement mode is set to channeling, tick is started if the skill has no cast time.
     * - Channeling not required: Abort() is called. */
    FSkillCastResult _endCast();

    bool _areTargetingConditionsVerifiedForTarget(TScriptInterface<ISkillTarget> target) const;
    bool _areCastConditionsVerified() const;

    enum class EMovementModeToSet : uint8
    {
        Default,
        Cast,
        Channeling,
    };
    void _setMovementModeIfPossible(EMovementModeToSet movementModeToSet) const;

    /** \brief Conditions that must be verified by the caster to cast the skill. */
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Casting")
    TArray<TObjectPtr<USkillCastCondition>> _castConditions{};

    /** \brief Conditions that must be verified by each target required by this skill. */
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Targeting")
    TArray<TObjectPtr<USkillTargetingCondition>> _targetingConditions{};

    /** \brief Kind of target needed by the skill to execute its logic. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting", meta = (MustImplement = "/Script/SkillSystem.SkillTarget"))
    TSubclassOf<UObject> _targetKind = nullptr;

    /** \brief Num of targets the skill needs to execute its logic. */
    UPROPERTY(EditDefaultsOnly, Category = "Targeting", meta = (ClampMin = "0"))
    int32 _nTargets = 0;

    UPROPERTY()
    TArray<TScriptInterface<ISkillTarget>> _targets{};

    TWeakObjectPtr<AActor> _caster = nullptr;
    TWeakObjectPtr<UManaComponent> _casterManaC = nullptr;
    TWeakInterfacePtr<IMovementCommandSetter> _casterMovementSetterC = nullptr;

    FTimerHandle _cooldownTimer{};

    bool _isTickAllowed = false;

    float _elapsedExecutionSeconds = 0.0f;
    float _castManaLeftToPay = 0.0f;

    FOnSkillStatusChanged _onSkillStatusChanged;

    TWeakObjectPtr<UTacticalPauseWorldSubsystem> _pauseSubSys = nullptr;
};
