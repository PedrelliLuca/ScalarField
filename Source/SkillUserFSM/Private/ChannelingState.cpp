// Fill out your copyright notice in the Description page of Project Settings.

#include "ChannelingState.h"

#include "CastingState.h"
#include "IdleState.h"
#include "MovementCommandSetter.h"
#include "SkillsContainerComponent.h"
#include "TargetingState.h"

TObjectPtr<USkillUserState> UChannelingState::OnTargeting(TObjectPtr<AActor> target, const TObjectPtr<AController> controller) {
    return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnBeginSkillExecution(TObjectPtr<UAbstractSkill> skill, const TObjectPtr<AController> controller) {
    if (skill == GetSkillInExecution()) {
        UE_LOG(LogTemp, Warning, TEXT("Skill is already being casted!"));
        return _keepCurrentState();
    }

    if (skill->IsOnCooldown()) {
        UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
        return _keepCurrentState();
    }

    TObjectPtr<UExecutionState> newState = nullptr;
    if (skill->RequiresTarget()) {
        newState = _abortExecutionForState<UTargetingState>(controller);
    } else {
        newState = _abortExecutionForState<UCastingState>(controller);
    }

    newState->SetSkillInExecution(skill);
    return newState;
}

TObjectPtr<USkillUserState> UChannelingState::OnTick(float deltaTime, const TObjectPtr<AController> controller) {
    const auto skill = GetSkillInExecution();
    const double channelingDuration = GetSkillInExecution()->GetChannelingTime();
    if (_elapsedChannelingTime + deltaTime >= channelingDuration) {
        // Apply one last mana payment with reduced deltaTime
        deltaTime = channelingDuration - _elapsedChannelingTime;
    }

    // No mana component == free skill
    if (_casterManaC.IsValid()) {
        const double charMana = _casterManaC->GetCurrentMana();

        const double manaCost = skill->GetChannelingManaCost();
        const double manaCostThisFrame = (deltaTime / channelingDuration) * manaCost;

        if (charMana < manaCostThisFrame) {
            UE_LOG(LogTemp, Error, TEXT("Not enough mana to keep channeling skill"));
            return _abortExecutionForState<UIdleState>(controller);
        }

        _casterManaC->SetCurrentMana(charMana - manaCostThisFrame);
    }

    // Execution of skill's channel logic
    skill->ExecuteChannelingTick(deltaTime, _caster.Get());
    _elapsedChannelingTime += deltaTime;

    if (FMath::IsNearlyEqual(_elapsedChannelingTime, channelingDuration)) {
        return _abortExecutionForState<UIdleState>(controller);
    }

    return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnAbort(const TObjectPtr<AController> controller) {
    UE_LOG(LogTemp, Error, TEXT("Skill channeling aborted!"));
    return _abortExecutionForState<UIdleState>(controller);
}

void UChannelingState::OnEnter(const TObjectPtr<AController> controller) {
    const auto skill = GetSkillInExecution();
    check(IsValid(skill) && skill->RequiresChanneling());
    UE_LOG(LogTemp, Warning, TEXT("Channeling..."));

    // Movement command update
    const auto movementSetters = controller->GetComponentsByInterface(UMovementCommandSetter::StaticClass());
    check(movementSetters.Num() == 1);
    const auto movementSetter = Cast<IMovementCommandSetter>(movementSetters[0]);
    check(movementSetter != nullptr);
    movementSetter->SetMovementMode(GetSkillInExecution()->GetChannelingMovementMode());

    _caster = controller->GetPawn();
    _casterManaC = _caster->FindComponentByClass<UManaComponent>();
}

void UChannelingState::OnLeave(const TObjectPtr<AController> controller) {
}
