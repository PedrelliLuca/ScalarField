// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillIconContainerWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/WrapBox.h"
#include "SkillIconWidget.h"
#include "SkillsContainerComponent.h"
#include "SkillsContainerInspector.h"

void USkillIconContainerWidget::BuildIconsFromContainer(
    TObjectPtr<USkillsContainerComponent> const& skillsContainer, TObjectPtr<UNewStateComponent> const& stateMachine) {
    _skillSlots->ClearChildren();
    _skillSlots->SetVisibility(ESlateVisibility::Visible);
    auto const containerInspector = FSkillsContainerInspector(skillsContainer);

    if (ensureMsgf(IsValid(_skillIconClass), TEXT(__FUNCTION__ ": Invalid Skill Icon Class, set it from Widget."))) {
        containerInspector.ForEachSkill([owner = GetOwningPlayer(), stateMachine, skillSlots = _skillSlots, skillIconClass = _skillIconClass](
                                            FSkillPropertiesInspector const& skillProp, int32 const skillIndex) {
            TObjectPtr<USkillIconWidget> const skillIconW = CreateWidget<USkillIconWidget>(owner, skillIconClass);
            skillIconW->InitFromSkillProperties(skillProp, stateMachine, skillIndex);
            skillSlots->AddChildToWrapBox(skillIconW);
        });
    }
}
