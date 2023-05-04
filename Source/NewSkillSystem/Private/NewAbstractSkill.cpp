// Fill out your copyright notice in the Description page of Project Settings.

#include "NewAbstractSkill.h"

FSkillCastResult UNewAbstractSkill::TryCast() {
    if (_onCooldown) {
        return FSkillCastResult::CastFail_Cooldown();
    }

    // TODO: if targets are required by this skill, check if all of them are available.

    if (!_areCastConditionsVerified()) {
        return FSkillCastResult::CastFail_CastConditionsViolated();
    }

    // TODO: if is casting or channeling, abort before casting.

    // TODO: cast immediately if _castSeconds is nearly zero.

    _isTickAllowed = true;
    _executionManaLeftToPay = _castManaCost + _channelingManaCost;
    _elapsedExecutionSeconds = 0.0f;

    // TODO: this should be something like FSkillCastResult::CastDeferred() if _castSeconds is not nearly zero, we're resorting to tick.
    return FSkillCastResult::CastSuccess();
}

void UNewAbstractSkill::SetCaster(const TObjectPtr<AActor> caster) {
    // Make sure that the caster is not valid before setting it.
    if (ensureMsgf(!_caster.IsValid(), TEXT("Caster can be set only once and has already been set."))) {
        check(IsValid(caster));
        _caster = caster;

        // We don't check(_casterManaC.IsValid()), casters are allowed to not have a mana component. In such case, they're special casters that do not pay mana.
        _casterManaC = _caster->FindComponentByClass<UManaComponent>();

        for (const auto castCondition : _castConditions) {
            castCondition->SetConditionSubject(caster);
        }
    }
}

void UNewAbstractSkill::Tick(const float deltaTime) {
    // Cast conditions must be verified during the entirity of the cast phase.
    if (!_areCastConditionsVerified()) {
        _isTickAllowed = false;
        _onCastPhaseFinish.Broadcast(FSkillCastResult::CastFail_CastConditionsViolated());
        return;
    }

    // TODO: distinguish between _castTick() and _channelingTick() phases here based on _elapsedExecutionSeconds.

    if (_elapsedExecutionSeconds + deltaTime >= _castSeconds) {
        if (_casterManaC.IsValid()) { // No mana component == free skill
            const auto currentMana = _casterManaC->GetCurrentMana();
            const auto manaLeftToPayForCast = _executionManaLeftToPay - _channelingManaCost;
            if (currentMana < manaLeftToPayForCast) {
                _casterManaC->SetCurrentMana(0.0f);
                _isTickAllowed = false;
                _onCastPhaseFinish.Broadcast(FSkillCastResult::CastFail_InsufficientMana());
                return;
            }
            _casterManaC->SetCurrentMana(currentMana - manaLeftToPayForCast);
        }

        // This function means that the actual casting can occurr.
        _cast();
        // TODO: don't set tick allowed to false if channeling is needed. Broadcast should signal whether channeling is needed or not.
        _isTickAllowed = false;
        GetWorld()->GetTimerManager().SetTimer(_cooldownTimer, this, &UNewAbstractSkill::_onCooldownEnded, _cooldownSeconds, false);
        _onCastPhaseFinish.Broadcast(FSkillCastResult::CastSuccess());
        return;
    }

    if (_casterManaC.IsValid()) { // No mana component == free skill
        const auto currentMana = _casterManaC->GetCurrentMana();

        const auto manaCost = _castManaCost + _channelingManaCost;
        const auto durationSeconds = _castSeconds + _channelingSeconds;

        const auto manaCostThisFrame = (deltaTime / durationSeconds) * manaCost;

        if (currentMana < manaCostThisFrame) {
            _casterManaC->SetCurrentMana(0.0f);
            _isTickAllowed = false;
            _onCastPhaseFinish.Broadcast(FSkillCastResult::CastFail_InsufficientMana());
            return;
        }

        _casterManaC->SetCurrentMana(currentMana - manaCostThisFrame);
        _executionManaLeftToPay -= manaCostThisFrame;
    }
    _elapsedExecutionSeconds += deltaTime;
}

bool UNewAbstractSkill::IsTickable() const {
    return _caster.IsValid();
}

bool UNewAbstractSkill::IsAllowedToTick() const {
    return _isTickAllowed;
}

bool UNewAbstractSkill::_areCastConditionsVerified() const {
    // TODO: When you'll have a proper error management system, this function will have to collect and return all errors thrown by the cast conditions.
    for (const auto castCondition : _castConditions) {
        if (!castCondition->IsVerified()) {
            return false;
        }
    }
    return true;
}

void UNewAbstractSkill::_onCooldownEnded() {
    _onCooldown = false;
}

void _abort() {
}
