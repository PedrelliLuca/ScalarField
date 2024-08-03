// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillIconContainerWidget.h"

#include "SkillsContainerComponent.h"
#include "SkillsContainerInspector.h"

void USkillIconContainerWidget::BuildIconsFromContainer(const TObjectPtr<USkillsContainerComponent> skillsContainer) {
    const auto containerInspector = FSkillsContainerInspector(skillsContainer);

    containerInspector.ForEachSkill([](const FSkillPropertiesInspector& skillProp) {
        // TODO: Build children
        return;
    });
}
