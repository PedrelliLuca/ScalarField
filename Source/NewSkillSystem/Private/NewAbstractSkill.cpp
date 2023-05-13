// Fill out your copyright notice in the Description page of Project Settings.

#include "NewAbstractSkill.h"

FSkillCastResult UNewAbstractSkill::TryCast() {
    check(_caster.IsValid());

    if (_onCooldown) {
        _setMovementModeIfPossible(EMovementModeToSet::Default);
        return FSkillCastResult::CastFail_Cooldown();
    }

    // TODO: if targets are required by this skill, check if all of them are available and return a specific FSkillCastResult kind.

    if (!_areCastConditionsVerified()) {
        _setMovementModeIfPossible(EMovementModeToSet::Default);
        return FSkillCastResult::CastFail_CastConditionsViolated();
    }

    // In case we're in cast or channeling phase, i.e. in case we're ticking, abort the old execution before starting the new one.
    if (_isTickAllowed) {
        return FSkillCastResult::CastFail_InExecution();
    }

    if (FMath::IsNearlyZero(_castSeconds)) {
        const auto currentMana = _casterManaC->GetCurrentMana();
        if (currentMana < _castManaCost) {
            _setMovementModeIfPossible(EMovementModeToSet::Default);
            return FSkillCastResult::CastFail_InsufficientMana();
        }
        _casterManaC->SetCurrentMana(currentMana - _castManaCost);

        _skillCast();
        GetWorld()->GetTimerManager().SetTimer(_cooldownTimer, this, &UNewAbstractSkill::_onCooldownEnded, _cooldownSeconds, false);
        _onCooldown = true;

        return _determineCastSuccessKind();
    }

    _isTickAllowed = true;
    _castManaLeftToPay = _castManaCost;
    _elapsedExecutionSeconds = 0.0f;

    _setMovementModeIfPossible(EMovementModeToSet::Cast);
    return FSkillCastResult::CastDeferred();
}

void UNewAbstractSkill::Abort() {
    // TODO: empty the targets' array here once you'll implement it.
    _isTickAllowed = false;
    _onCastPhaseEnd.Clear();
    _onChannelingPhaseEnd.Clear();
    _setMovementModeIfPossible(EMovementModeToSet::Default);
    _skillAbort();
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

        if (const auto casterAsPawn = Cast<APawn>(caster); IsValid(casterAsPawn)) {
            // TODO: update this once the IMovementCommandSetter will be on the pawn
            if (const auto casterController = casterAsPawn->GetController(); IsValid(casterController)) {
                // We don't check(_casterMovementSetterC.IsValid()), casters are allowed to not have a movement setter. In such case, the skill won't change
                // their movement mode.
                _casterMovementSetterC = Cast<IMovementCommandSetter>(casterController->FindComponentByInterface<UMovementCommandSetter>());
            }
        }
    }
}

void UNewAbstractSkill::Tick(const float deltaTime) {
    if (!_pauseSubSys.IsValid()) {
        _pauseSubSys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
        check(_pauseSubSys.IsValid());
    }

    // Unfortunately, setting the global time dilation doesn't work with FTickableGameObjects. We have to manually pause ticking like follows.
    if (_pauseSubSys->IsTacticalPauseOn()) {
        return;
    }

    if (_elapsedExecutionSeconds < _castSeconds) {
        _castTick(deltaTime);
    } else if (_elapsedExecutionSeconds < _castSeconds + _channelingSeconds) {
        _channelingTick(deltaTime);
    } else {
        // You should've stopped ticking, some reset hasn't gone well.
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
        _onCastPhaseEnd.Broadcast(FSkillCastResult::CastFail_CastConditionsViolated());
        Abort();
        return;
    }

    // Last tick in cast phase
    if (_elapsedExecutionSeconds + deltaTime >= _castSeconds) {
        if (_casterManaC.IsValid()) { // No mana component == free skill
            const auto currentMana = _casterManaC->GetCurrentMana();
            if (currentMana < _castManaLeftToPay) {
                _casterManaC->SetCurrentMana(0.0f);
                
                _onCastPhaseEnd.Broadcast(FSkillCastResult::CastFail_InsufficientMana());
                Abort();
                return;
            }
            _casterManaC->SetCurrentMana(currentMana - _castManaLeftToPay);
        }

        _skillCast();
        GetWorld()->GetTimerManager().SetTimer(_cooldownTimer, this, &UNewAbstractSkill::_onCooldownEnded, _cooldownSeconds, false);
        _onCooldown = true;

        auto endCastResult = _determineCastSuccessKind();
        _onCastPhaseEnd.Broadcast(MoveTemp(endCastResult));
        _onCastPhaseEnd.Clear();

        _elapsedExecutionSeconds += _castSeconds - _elapsedExecutionSeconds;
        return;
    }

    if (_casterManaC.IsValid()) { // No mana component == free skill
        const auto currentMana = _casterManaC->GetCurrentMana();
        const auto manaCostThisFrame = (deltaTime / _castSeconds) * _castManaCost;

        if (currentMana < manaCostThisFrame) {
            _casterManaC->SetCurrentMana(0.0f);
            
            _onCastPhaseEnd.Broadcast(FSkillCastResult::CastFail_InsufficientMana());
            Abort();
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

            _onChannelingPhaseEnd.Broadcast(FSkillChannelingResult::ChannelingFail_InsufficientMana());
            Abort();
            return;
        }

        _casterManaC->SetCurrentMana(currentMana - manaCostThisFrame);
    }

    _skillChannelingTick(deltaTime);
    _elapsedExecutionSeconds += deltaTime;

    if (FMath::IsNearlyEqual(_elapsedExecutionSeconds, executionSeconds)) {
        _onChannelingPhaseEnd.Broadcast(FSkillChannelingResult::ChannelingSuccess());
        Abort();
    }
}

FSkillCastResult UNewAbstractSkill::_determineCastSuccessKind() {
    if (_channelingSeconds > 0.0f) {
        // This skill requires channeling
        _isTickAllowed = true;
        _setMovementModeIfPossible(EMovementModeToSet::Channeling);
        return FSkillCastResult::CastSuccess_IntoChanneling();
    }

    // This skill does not require channeling
    _isTickAllowed = false;
    _setMovementModeIfPossible(EMovementModeToSet::Default);
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

void UNewAbstractSkill::_setMovementModeIfPossible(const EMovementModeToSet movementModeToSet) const {
    // It's ok for the caster to not have a movement setter. In such case, the skill won't affect its movement.
    if (!_casterMovementSetterC.IsValid()) {
        return;
    }

    switch (movementModeToSet) {
        case EMovementModeToSet::Default:
            _casterMovementSetterC->SetDefaultMovementMode();
            break;
        case EMovementModeToSet::Cast:
            _casterMovementSetterC->SetMovementMode(_castMovementMode);
            break;
        case EMovementModeToSet::Channeling:
            _casterMovementSetterC->SetMovementMode(_channelingMovementMode);
            break;
    }
}
