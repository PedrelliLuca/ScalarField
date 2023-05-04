// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ManaComponent.h"
#include "SkillCastCondition.h"
#include "SkillCastResult.h"

#include "NewAbstractSkill.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCastPhaseFinish, FSkillCastResult);

/** A skill of the ScalarField game. This can't be an interface because it's meant to be employed with the "Instanced" UPROPERTY specifier, see
 * UNewSkillsContainerComponent. */
UCLASS(NotBlueprintable, Abstract, DefaultToInstanced)
class NEWSKILLSYSTEM_API UNewAbstractSkill : public UObject, public FTickableGameObject {
    GENERATED_BODY()

public:
    FSkillCastResult TryCast();

    void SetCaster(TObjectPtr<AActor> caster);

    void Tick(float deltaTime) override;

    /** \brief Determines whether this skill is ready to tick or not by checking whether the skill's caster is valid or not. */
    bool IsTickable() const override;

    /** \brief Determines whether this skill can tick or not. Skills are allowed to tick while they're in execution, i.e. when they're being cast or channeled.
     */
    bool IsAllowedToTick() const override;

    /** \brief We have to override this because it's virtual pure in FTickableGameObject, but in practice it's not needed. */
    TStatId GetStatId() const override { return TStatId{}; }

    FOnCastPhaseFinish& OnCastPhaseFinish() { return _onCastPhaseFinish; }

protected:
    /** \brief The time it takes for the skill to be castable again. The countdown starts from the moment the cast is complete. */
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0"))
    float _cooldownSeconds = 1.0f;

    /** \brief The cost in mana to both cast and channel the skill. */
    UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0"))
    float _totalManaCost = 50.0f;

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

private:
    /** \brief Represents the concrete cast of the skill, skill-specific logic for the cast is executed in here. */
    virtual void _cast() PURE_VIRTUAL(UNewAbstractSkill::_cast, return;);

    void _abort();

    bool _areCastConditionsVerified() const;

    void _onCooldownEnded();

    /** \brief Conditions that must be verified to cast the skill. */
    UPROPERTY(EditDefaultsOnly, Instanced, Category = "Conditions")
    TArray<TObjectPtr<USkillCastCondition>> _castConditions{};

    TWeakObjectPtr<AActor> _caster = nullptr;
    TWeakObjectPtr<UManaComponent> _casterManaC = nullptr;

    FTimerHandle _cooldownTimer{};
    bool _onCooldown = false;

    bool _isTickAllowed = false;

    float _elapsedExecutionSeconds = 0.0f;
    float _executionManaLeftToPay = 0.0f;

    FOnCastPhaseFinish _onCastPhaseFinish{};
};
