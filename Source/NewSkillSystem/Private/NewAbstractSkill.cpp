// Fill out your copyright notice in the Description page of Project Settings.

#include "NewAbstractSkill.h"

FSkillCastResult UNewAbstractSkill::TryCast() {
    check(_caster.IsValid());

    if (_onCooldown) {
        return FSkillCastResult::CastFail_Cooldown();
    }

    // TODO: if targets are required by this skill, check if all of them are available and return a specific FSkillCastResult kind.

    if (!_areCastConditionsVerified()) {
        return FSkillCastResult::CastFail_CastConditionsViolated();
    }

    // TODO: if is casting or channeling, abort before casting.

    if (FMath::IsNearlyZero(_castSeconds)) {
        _skillCast();
        GetWorld()->GetTimerManager().SetTimer(_cooldownTimer, this, &UNewAbstractSkill::_onCooldownEnded, _cooldownSeconds, false);
        return _determineCastSuccessKind();
    }

    _isTickAllowed = true;
    _castManaLeftToPay = _castManaCost;
    _elapsedExecutionSeconds = 0.0f;

    return FSkillCastResult::CastDeferred();
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
    if (_elapsedExecutionSeconds < _castSeconds) {
        _castTick(deltaTime);
    } else if (_elapsedExecutionSeconds < _castSeconds + _channelingSeconds) {
        _channelingTick(deltaTime);
    } else {
        // You should've stopped tick, some reset hasn't gone well.
        checkNoEntry();
    }
}

bool UNewAbstractSkill::IsTickable() const {
    return _caster.IsValid();
}

bool UNewAbstractSkill::IsAllowedToTick() const {
    return _isTickAllowed;
}

void UNewAbstractSkill::_castTick(const float deltaTime) {
    // Cast conditions must be verified during the entire cast phase.
    if (!_areCastConditionsVerified()) {
        _isTickAllowed = false;
        _onCastPhaseFinish.Broadcast(FSkillCastResult::CastFail_CastConditionsViolated());
        return;
    }

    // Last tick in cast phase
    if (_elapsedExecutionSeconds + deltaTime >= _castSeconds) {
        if (_casterManaC.IsValid()) { // No mana component == free skill
            const auto currentMana = _casterManaC->GetCurrentMana();
            if (currentMana < _castManaLeftToPay) {
                _casterManaC->SetCurrentMana(0.0f);
                _isTickAllowed = false;
                _onCastPhaseFinish.Broadcast(FSkillCastResult::CastFail_InsufficientMana());
                return;
            }
            _casterManaC->SetCurrentMana(currentMana - _castManaLeftToPay);
        }

        _skillCast();
        GetWorld()->GetTimerManager().SetTimer(_cooldownTimer, this, &UNewAbstractSkill::_onCooldownEnded, _cooldownSeconds, false);
        auto endCastResult = _determineCastSuccessKind();
        _onCastPhaseFinish.Broadcast(MoveTemp(endCastResult));

        _elapsedExecutionSeconds += _castSeconds - _elapsedExecutionSeconds;
        return;
    }

    if (_casterManaC.IsValid()) { // No mana component == free skill
        const auto currentMana = _casterManaC->GetCurrentMana();
        const auto manaCostThisFrame = (deltaTime / _castSeconds) * _castManaCost;

        if (currentMana < manaCostThisFrame) {
            _casterManaC->SetCurrentMana(0.0f);
            _isTickAllowed = false;
            _onCastPhaseFinish.Broadcast(FSkillCastResult::CastFail_InsufficientMana());
            return;
        }

        _casterManaC->SetCurrentMana(currentMana - manaCostThisFrame);
        _castManaLeftToPay -= manaCostThisFrame;
    }
    _elapsedExecutionSeconds += deltaTime;
}

void UNewAbstractSkill::_channelingTick(float deltaTime) {
    // TODO: implement channeling conditions

    const auto executionSeconds = _castSeconds + _channelingSeconds;
    if (_elapsedExecutionSeconds + deltaTime >= executionSeconds) {
        // Apply one last mana payment with reduced deltaTime
        deltaTime = executionSeconds - _elapsedExecutionSeconds;
    }

    // No mana component == free skill
    if (_casterManaC.IsValid()) {
        const double currentMana = _casterManaC->GetCurrentMana();
        const double manaCostThisFrame = (deltaTime / _channelingSeconds) * _channelingManaCost;

        if (currentMana < manaCostThisFrame) {
            _casterManaC->SetCurrentMana(0.0f);
            _isTickAllowed = false;
            _onChannelingPhaseFinish.Broadcast(FSkillChannelingResult::ChannelingFail_InsufficientMana());
            return;
        }

        _casterManaC->SetCurrentMana(currentMana - manaCostThisFrame);
    }

    _skillChannelingTick(deltaTime);
    _elapsedExecutionSeconds += deltaTime;

    if (FMath::IsNearlyEqual(_elapsedExecutionSeconds, executionSeconds)) {
        _isTickAllowed = false;
        _onChannelingPhaseFinish.Broadcast(FSkillChannelingResult::ChannelingSuccess());
    }
}

FSkillCastResult UNewAbstractSkill::_determineCastSuccessKind() {
    if (_channelingSeconds > 0.0f) {
        // This skill requires channeling
        _isTickAllowed = true;
        return FSkillCastResult::CastSuccess_IntoChanneling();
    }

    // This skill does not require channeling
    _isTickAllowed = false;
    return FSkillCastResult::CastSuccess_IntoExecutionEnd();
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
