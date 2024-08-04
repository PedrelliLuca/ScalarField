// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillIconContainerWidget.h"

#include "Components/WrapBox.h"
#include "SkillIconWidget.h"
#include "SkillsContainerComponent.h"
#include "SkillsContainerInspector.h"

void USkillIconContainerWidget::BuildIconsFromContainer(const TObjectPtr<USkillsContainerComponent>& skillsContainer) {
    _skillSlots->ClearChildren();
    _skillSlots->SetVisibility(ESlateVisibility::Visible);
    const auto containerInspector = FSkillsContainerInspector(skillsContainer);

    containerInspector.ForEachSkill([owner = GetOwningPlayer(), skillSlots = _skillSlots](const FSkillPropertiesInspector& skillProp) {
        const TObjectPtr<USkillIconWidget> skillIconW = CreateWidget<USkillIconWidget>(owner, USkillIconWidget::StaticClass());
        skillIconW->SetVisibility(ESlateVisibility::Visible);
        skillSlots->AddChildToWrapBox(skillIconW);
    });
}
