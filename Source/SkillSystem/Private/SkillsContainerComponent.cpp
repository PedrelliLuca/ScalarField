// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsContainerComponent.h"

#include "NewIceWallSkill.h"

USkillsContainerComponent::USkillsContainerComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillsContainerComponent::BeginPlay() {
	Super::BeginPlay();

	for (const auto skillParametersClass : _skillsParametersClasses) {
		_skills.Emplace(_createSkill(skillParametersClass));
	}
}

TObjectPtr<UNewAbstractSkill> USkillsContainerComponent::_createSkill(const TSubclassOf<UNewSkillParameters> skillParametersClass) const {
	const auto parameters = NewObject<UNewSkillParameters>(GetOwner(), skillParametersClass);
	
	TObjectPtr<UClass> skillClass;

	switch (parameters->Skill) {
	case EConcreteSkill::CS_IceWall:
		skillClass = UNewIceWallSkill::StaticClass();
		break;
	default:
		checkNoEntry();
	};

	const auto skill = NewObject<UNewAbstractSkill>(GetOwner(), skillClass);
	skill->InitializeSkill(parameters);

	return skill;
}
