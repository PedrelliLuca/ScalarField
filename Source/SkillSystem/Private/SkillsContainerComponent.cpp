// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsContainerComponent.h"

#include "ManaComponent.h"

USkillsContainerComponent::USkillsContainerComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

bool USkillsContainerComponent::ExecuteSkillAtIndex(const uint32 index) {
	const auto owner = GetOwner();

	if (!_skills.IsValidIndex(index)) {
		UE_LOG(LogTemp, Error, TEXT("There is no skill at index %i"), index);
		return false;
	}

	const auto skill = _skills[index];
	check(skill != nullptr);

	if (skill->IsOnCooldown()) {
		UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
		return false;
	}

	if (const auto manaC = owner->FindComponentByClass<UManaComponent>()) {
		const double charMana = manaC->GetMana();
		const double manaCost = skill->GetManaCost();
		if (charMana < manaCost) {
			UE_LOG(LogTemp, Error, TEXT("Not enough mana to cast skill at index %i"), index);
			return false;
		}

		manaC->SetMana(charMana - manaCost);
	}

	// The owner isn't forced to have a mana component. If it doesn't have one, it means that it can cast its skills for free.
	// Elements in the environment, like turrets that spit fire or clouds that spawn lightning bolts, are examples of this.
	skill->Execute(owner);

	return true;
}

void USkillsContainerComponent::BeginPlay() {
	Super::BeginPlay();

	for (const auto skillClass : _skillsClasses) {
		const auto skill = NewObject<UAbstractSkill>(GetOwner(), skillClass);

#if DO_CHECK
		skill->CheckParametersSanity();
#endif

		_skills.Emplace(skill);
	}
}
