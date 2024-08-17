// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillIconContainerWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/WrapBox.h"
#include "SkillIconWidget.h"
#include "SkillsContainerComponent.h"
#include "SkillsContainerInspector.h"

void USkillIconContainerWidget::BuildIconsFromContainer(
    const TObjectPtr<USkillsContainerComponent>& skillsContainer, const TObjectPtr<UNewStateComponent>& stateMachine) {
    _skillSlots->ClearChildren();
    _skillSlots->SetVisibility(ESlateVisibility::Visible);
    const auto containerInspector = FSkillsContainerInspector(skillsContainer);

    if (ensureMsgf(IsValid(_skillIconClass), TEXT(__FUNCTION__ ": Invalid Skill Icon Class, set it from Widget."))) {
        containerInspector.ForEachSkill([owner = GetOwningPlayer(), stateMachine, skillSlots = _skillSlots, skillIconClass = _skillIconClass](
                                            const FSkillPropertiesInspector& skillProp, const int32 skillIndex) {
            const TObjectPtr<USkillIconWidget> skillIconW = CreateWidget<USkillIconWidget>(owner, skillIconClass);
            skillIconW->InitFromSkillProperties(skillProp, stateMachine, skillIndex);
            skillSlots->AddChildToWrapBox(skillIconW);
        });
    }
}