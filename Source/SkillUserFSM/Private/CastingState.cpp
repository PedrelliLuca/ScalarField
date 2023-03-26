// Fill out your copyright notice in the Description page of Project Settings.

#include "CastingState.h"

#include "ChannelingState.h"
#include "IdleState.h"
#include "MovementCommandSetter.h"
#include "SkillsContainerComponent.h"
#include "TargetingState.h"

TObjectPtr<USkillUserState> UCastingState::OnTargeting(const TObjectPtr<AActor> target, const TObjectPtr<AController> controller) {
    return _keepCurrentState();
}

TObjectPtr<USkillUserState> UCastingState::OnBeginSkillExecution(TObjectPtr<UAbstractSkill> skill, const TObjectPtr<AController> controller) {
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

TObjectPtr<USkillUserState> UCastingState::OnTick(const float deltaTime, const TObjectPtr<AController> controller) {
    const auto skill = GetSkillInExecution();
    const double castDuration = GetSkillInExecution()->GetCastTime();

    if (_elapsedCastTime + deltaTime >= castDuration) {
        if (_casterManaC.IsValid()) { // No mana component == free skill
            const auto currentMana = _casterManaC->GetCurrentMana();

            if (currentMana < _manaLeftToPay) {
                UE_LOG(LogTemp, Error, TEXT("Not enough mana to keep casting skill"));
                return _abortExecutionForState<UIdleState>(controller);
            }
            _casterManaC->SetCurrentMana(currentMana - _manaLeftToPay);
        }

        skill->ExecuteCast(_caster.Get());
        return _determineStateBasedOnSkillChanneling(skill, controller);
    }

    if (_casterManaC.IsValid()) { // No mana component == free skill
        const auto currentMana = _casterManaC->GetCurrentMana();

        const double manaCost = skill->GetCastManaCost();
        const double manaCostThisFrame = (deltaTime / castDuration) * manaCost;

        if (currentMana < manaCostThisFrame) {
            UE_LOG(LogTemp, Error, TEXT("Not enough mana to keep casting skill"));
            return _abortExecutionForState<UIdleState>(controller);
        }

        _casterManaC->SetCurrentMana(currentMana - manaCostThisFrame);
        _manaLeftToPay -= manaCostThisFrame;
    }

    _elapsedCastTime += deltaTime;
    return _keepCurrentState();
}

TObjectPtr<USkillUserState> UCastingState::OnAbort(const TObjectPtr<AController> controller) {
    UE_LOG(LogTemp, Error, TEXT("Skill cast aborted!"));
    return _abortExecutionForState<UIdleState>(controller);
}

void UCastingState::OnEnter(const TObjectPtr<AController> controller) {
    UE_LOG(LogTemp, Warning, TEXT("Skill cast begun!"));

    // Movement command update
    const auto movementSetters = controller->GetComponentsByInterface(UMovementCommandSetter::StaticClass());
    check(movementSetters.Num() == 1);
    const auto movementSetter = Cast<IMovementCommandSetter>(movementSetters[0]);
    check(movementSetter != nullptr);
    movementSetter->SetMovementMode(GetSkillInExecution()->GetCastMovementMode());

    _caster = controller->GetPawn();
    _casterManaC = _caster->FindComponentByClass<UManaComponent>();

    const auto skill = GetSkillInExecution();
    _manaLeftToPay = skill->GetCastManaCost();
}

void UCastingState::OnLeave(const TObjectPtr<AController> controller) {
}

TObjectPtr<USkillUserState> UCastingState::_determineStateBasedOnSkillChanneling(
    const TObjectPtr<UAbstractSkill> skill, const TObjectPtr<AController> controller) {
    if (GetSkillInExecution()->RequiresChanneling()) {
        const auto channelingState = NewObject<UChannelingState>(controller, UChannelingState::StaticClass());
        channelingState->SetSkillInExecution(skill);
        return channelingState;
    }

    return NewObject<UIdleState>(controller);
}
