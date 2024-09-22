// Fill out your copyright notice in the Description page of Project Settings.

#include "AbstractSkill.h"

FSkillCastResult UAbstractSkill::TryCast() {
    check(_caster.IsValid());

    bool const isOnCooldown = GetWorld()->GetTimerManager().IsTimerActive(_cooldownTimer);
    if (isOnCooldown) {
        _setMovementModeIfPossible(EMovementModeToSet::Default);
        return FSkillCastResult::CastFail_Cooldown();
    }

    if (!_areCastConditionsVerified()) {
        _setMovementModeIfPossible(EMovementModeToSet::Default);
        return FSkillCastResult::CastFail_CastConditionsViolated();
    }

    // In case we're in cast or channeling phase, i.e. in case we're ticking, abort the old execution before starting the new one.
    if (_isTickAllowed) {
        return FSkillCastResult::CastFail_InExecution();
    }

    // TODO: if targets are required by this skill, check if all of them are available and return a specific FSkillCastResult kind.
    if (_targets.Num() != _nTargets) {
        _onSkillStatusChanged.Broadcast(ESkillStatus::Targeting);

        // If we have more targets then we can handle, something went horribly wrong.
        check(_targets.Num() < _nTargets);
        return FSkillCastResult::CastFail_MissingTarget();
    }

    if (FMath::IsNearlyZero(_castSeconds)) {
        auto const currentMana = _casterManaC->GetCurrentMana();
        if (currentMana < _castManaCost) {
            _setMovementModeIfPossible(EMovementModeToSet::Default);
            return FSkillCastResult::CastFail_InsufficientMana();
        }
        _casterManaC->SetCurrentMana(currentMana - _castManaCost);

        _skillCast();

        auto const castResult = _endCast();
        return castResult;
    }

    // Skills that have a casting phase need this kind of setup
    _isTickAllowed = true;
    _castManaLeftToPay = _castManaCost;
    _elapsedExecutionSeconds = 0.0f;

    _setMovementModeIfPossible(EMovementModeToSet::Cast);
    _onSkillStatusChanged.Broadcast(ESkillStatus::Casting);
    return FSkillCastResult::CastDeferred();
}

void UAbstractSkill::Abort(bool const shouldResetMovement) {
    // An abort prompted by clients can start the skill's cooldown only if the skill is channeling.
    bool const isChanneling = _castSeconds < _elapsedExecutionSeconds && _elapsedExecutionSeconds < _castSeconds + _channelingSeconds;
    _abort(shouldResetMovement, isChanneling);
}

FSkillTargetingResult UAbstractSkill::TryAddTarget(FSkillTargetPacket const& targetPacket) {
    if (_targets.Num() == _nTargets) {
        return FSkillTargetingResult::TargetingFail_AlreadyAvailableTargets();
    }

    TScriptInterface<ISkillTarget> target = NewObject<UObject>(this, _targetKind);
    target->Init(targetPacket);

    if (target->IsValidTarget() && _areTargetingConditionsVerifiedForTarget(target)) {
        _targets.Emplace(MoveTemp(target));

        check(_targets.Num() <= _nTargets);
        if (_targets.Num() == _nTargets) {
            _onSkillStatusChanged.Broadcast(ESkillStatus::Casting);
            return FSkillTargetingResult::TargetingSuccess_IntoCast();
        }

        return FSkillTargetingResult::TargetingSuccess_KeepTargeting();
    }

    return FSkillTargetingResult::TargetingFail_InvalidTarget();
}

void UAbstractSkill::SetCaster(TObjectPtr<AActor> const caster) {
    // Make sure that the caster is not valid before setting it.
    if (ensureMsgf(!_caster.IsValid(), TEXT("Caster can be set only once and has already been set."))) {
        check(IsValid(caster));
        _caster = caster;

        // We don't check(_casterManaC.IsValid()), casters are allowed to not have a mana component. In such case, they're special casters that do not pay mana.
        _casterManaC = _caster->FindComponentByClass<UManaComponent>();

        for (auto const targetingCondition : _targetingConditions) {
            targetingCondition->SetSkillCaster(caster);
        }

        for (auto const castCondition : _castConditions) {
            castCondition->SetSkillCaster(caster);
        }

        if (auto const casterAsPawn = Cast<APawn>(caster); IsValid(casterAsPawn)) {
            // TODO: update this once the IMovementCommandSetter will be on the pawn
            if (auto const casterController = casterAsPawn->GetController(); IsValid(casterController)) {
                // We don't check(_casterMovementSetterC.IsValid()), casters are allowed to not have a movement setter. In such case, the skill won't change
                // their movement mode.
                _casterMovementSetterC = Cast<IMovementCommandSetter>(casterController->FindComponentByInterface<UMovementCommandSetter>());
            }
        }
    }
}

void UAbstractSkill::Tick(float const deltaTime) {
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

bool UAbstractSkill::IsTickable() const {
    return _caster.IsValid();
}

bool UAbstractSkill::IsAllowedToTick() const {
    return _isTickAllowed;
}

void UAbstractSkill::_abort(bool const shouldResetMovement, bool const shouldStartCooldownTimer) {
    if (shouldResetMovement) {
        _setMovementModeIfPossible(EMovementModeToSet::Default);
    }

    _isTickAllowed = false;
    _elapsedExecutionSeconds = 0.0f;
    _targets.Empty();
    _skillAbort();

    if (shouldStartCooldownTimer) {
        check(!GetWorld()->GetTimerManager().IsTimerActive(_cooldownTimer));

        auto cooldownTimerCallback = [this]() { _onSkillStatusChanged.Broadcast(ESkillStatus::None); };
        GetWorld()->GetTimerManager().SetTimer(_cooldownTimer, MoveTemp(cooldownTimerCallback), _cooldownSeconds, false);
        _onSkillStatusChanged.Broadcast(ESkillStatus::Cooldown);
    } else {
        _onSkillStatusChanged.Broadcast(ESkillStatus::None);
    }
}

void UAbstractSkill::_castTick(float const deltaTime) {
    // Targeting conditions must be verified during the entire cast phase.
    for (auto const& target : _targets) {
        if (!target->IsValidTarget() || !_areTargetingConditionsVerifiedForTarget(target)) {
            _abort(true, false);
            return;
        }
    }

    // Cast conditions must be verified during the entire cast phase.
    if (!_areCastConditionsVerified()) {
        _abort(true, false);
        return;
    }

    // Last tick in cast phase
    if (_elapsedExecutionSeconds + deltaTime >= _castSeconds) {
        if (_casterManaC.IsValid()) { // No mana component == free skill
            auto const currentMana = _casterManaC->GetCurrentMana();
            if (currentMana < _castManaLeftToPay) {
                _casterManaC->SetCurrentMana(0.0f);

                _abort(true, false);
                return;
            }
            _casterManaC->SetCurrentMana(currentMana - _castManaLeftToPay);
        }

        _skillCast();
        _endCast();
        _elapsedExecutionSeconds += _castSeconds - _elapsedExecutionSeconds;
        return;
    }

    if (_casterManaC.IsValid()) { // No mana component == free skill
        auto const currentMana = _casterManaC->GetCurrentMana();
        auto const manaCostThisFrame = (deltaTime / _castSeconds) * _castManaCost;

        if (currentMana < manaCostThisFrame) {
            _casterManaC->SetCurrentMana(0.0f);
            _abort(true, false);
            return;
        }

        _casterManaC->SetCurrentMana(currentMana - manaCostThisFrame);
        _castManaLeftToPay -= manaCostThisFrame;
    }
    _elapsedExecutionSeconds += deltaTime;
}

void UAbstractSkill::_channelingTick(float deltaTime) {
    if (_checkTargetingConditionsWhenChanneling) {
        // Targeting conditions must be verified during the entire cast phase.
        for (auto const& target : _targets) {
            if (!target->IsValidTarget() || !_areTargetingConditionsVerifiedForTarget(target)) {
                _abort(true, true);
                return;
            }
        }
    }

    // TODO: implement channeling conditions

    auto const executionSeconds = _castSeconds + _channelingSeconds;
    if (_elapsedExecutionSeconds + deltaTime >= executionSeconds) {
        // Apply one last mana payment with reduced deltaTime
        deltaTime = executionSeconds - _elapsedExecutionSeconds;
    }

    // No mana component == free skill
    if (_casterManaC.IsValid()) {
        double const currentMana = _casterManaC->GetCurrentMana();
        double const manaCostThisFrame = (deltaTime / _channelingSeconds) * _channelingManaCost;

        if (currentMana < manaCostThisFrame) {
            _casterManaC->SetCurrentMana(0.0f);
            _abort(true, true);
            return;
        }

        _casterManaC->SetCurrentMana(currentMana - manaCostThisFrame);
    }

    _skillChannelingTick(deltaTime);
    _elapsedExecutionSeconds += deltaTime;

    if (FMath::IsNearlyEqual(_elapsedExecutionSeconds, executionSeconds)) {
        _abort(true, true);
    }
}

FSkillCastResult UAbstractSkill::_endCast() {
    if (_channelingSeconds > 0.0f) {             // This skill requires channeling
        if (FMath::IsNearlyZero(_castSeconds)) { // This skill cast was immediate
            _elapsedExecutionSeconds = 0.0f;
        }

        _isTickAllowed = true;

        _setMovementModeIfPossible(EMovementModeToSet::Channeling);
        _onSkillStatusChanged.Broadcast(ESkillStatus::Channeling);
        return FSkillCastResult::CastSuccess_IntoChanneling();
    }

    // This skill does not require channeling => it's over!
    _abort(true, true);
    return FSkillCastResult::CastSuccess_IntoExecutionEnd();
}

bool UAbstractSkill::_areTargetingConditionsVerifiedForTarget(TScriptInterface<ISkillTarget> target) const {
    // TODO: When you'll have a proper error management system, this function will have to collect and return all errors thrown by the cast conditions.
    for (auto const targetingCondition : _targetingConditions) {
        if (!targetingCondition->IsVerifiedForTarget(target.GetInterface())) {
            return false;
        }
    }
    return true;
}

bool UAbstractSkill::_areCastConditionsVerified() const {
    // TODO: When you'll have a proper error management system, this function will have to collect and return all errors thrown by the cast conditions.
    for (auto const castCondition : _castConditions) {
        if (!castCondition->IsVerified()) {
            return false;
        }
    }
    return true;
}

void UAbstractSkill::_setMovementModeIfPossible(EMovementModeToSet const movementModeToSet) const {
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
