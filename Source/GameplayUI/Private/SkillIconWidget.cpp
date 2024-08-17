// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillIconWidget.h"

#include "AbstractSkill.h"
#include "Components/Image.h"
#include "SkillPropertiesInspector.h"

void USkillIconWidget::InitFromSkillProperties(const FSkillPropertiesInspector& skillProp) {
    _skillCooldownSeconds = skillProp.GetCooldownSeconds();
    _skillThumbnail->SetBrushFromTexture(skillProp.GetSkillThumbnail());

    auto skillStatusChangedCallback = [this](const ESkillStatus newStatus) {
        _onSkillStatusChanged(newStatus);

        if (newStatus == ESkillStatus::Cooldown) {
            _setupCooldownTimer(newStatus);
        }
    };

    skillProp.OnSkillStatusChanged().AddLambda(MoveTemp(skillStatusChangedCallback));
}

void USkillIconWidget::_setupCooldownTimer(const ESkillStatus newStatus) {
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
