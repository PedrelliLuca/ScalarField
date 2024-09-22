// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillIconWidget.h"

#include "AbstractSkill.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "NewStateComponent.h"
#include "SkillPropertiesInspector.h"

void USkillIconWidget::InitFromSkillProperties(
    FSkillPropertiesInspector const& skillProp, TObjectPtr<UNewStateComponent> const& stateMachine, int32 const skillIndex) {
    _skillCooldownSeconds = skillProp.GetCooldownSeconds();
    _skillThumbnail->SetBrushFromTexture(skillProp.GetSkillThumbnail());

    auto skillStatusChangedCallback = [this](ESkillStatus const newStatus) {
        _onSkillStatusChanged(newStatus);

        if (newStatus == ESkillStatus::Cooldown) {
            _setupCooldownTimer(newStatus);
        }
    };

    skillProp.OnSkillStatusChanged().AddLambda(MoveTemp(skillStatusChangedCallback));

    if (IsValid(stateMachine)) {
        _hudOwnerState = stateMachine;
        _skillIndex = skillIndex;
        _skillExecutionButton->OnReleased.AddDynamic(this, &USkillIconWidget::_tryCastSkill);
    }
}

void USkillIconWidget::_setupCooldownTimer(ESkillStatus const newStatus) {
    FTimerManager& timerManager = GetWorld()->GetTimerManager();

    // We can't be entering cooldown when we're already on cooldown, something went horribly wrong (either here or in UAbstractSkill).
    check(!timerManager.IsTimerActive(_cooldownTimer));

    float cooldownTimeRemaining = _skillCooldownSeconds;
    _onCooldownUpdate(cooldownTimeRemaining, true);

    auto cooldownUpdateCallback = [this, cooldownTimeRemaining]() mutable {
        cooldownTimeRemaining -= _intervalBetweenCooldownUpdates;
        _onCooldownUpdate(cooldownTimeRemaining, false);

        if (cooldownTimeRemaining <= 0.0f) {
            GetWorld()->GetTimerManager().ClearTimer(_cooldownTimer);
        }
    };

    timerManager.SetTimer(_cooldownTimer, MoveTemp(cooldownUpdateCallback), _intervalBetweenCooldownUpdates, true);
}

void USkillIconWidget::_tryCastSkill() {
    if (_hudOwnerState.IsValid()) {
        check(_skillIndex.IsSet());
        _hudOwnerState->TryCastSkillAtIndex(_skillIndex.GetValue());
    }

    UWidgetBlueprintLibrary::SetFocusToGameViewport();
}
