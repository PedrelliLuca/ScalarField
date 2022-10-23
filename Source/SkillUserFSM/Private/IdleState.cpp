// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

#include "CastingState.h"
#include "ManaComponent.h"
#include "SkillsContainerComponent.h"

TObjectPtr<USkillUserState> UIdleState::OnTargeting(TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UIdleState::OnBeginSkillExecution(const int32 skillKey, TObjectPtr<AController> controller) {
	check(skillKey < KEY_ASSIGNABLE_SKILLS);
	const auto pawn = controller->GetPawn();
	const auto skillsContainer = pawn->FindComponentByClass<USkillsContainerComponent>();
	check(IsValid(skillsContainer));

	// keys [1, 2, ..., 9, 0] => index [0, 1, ..., 8, 9]
	const uint32 index = skillKey != 0 ? skillKey - 1 : KEY_ASSIGNABLE_SKILLS - 1;
	const auto skill = skillsContainer->GetSkillAtIndex(index);

	if (!IsValid(skill)) {
		UE_LOG(LogTemp, Warning, TEXT("Skill bound with key &i isn't valid!"), skillKey);
		return _keepCurrentState();
	}

	if (skill->IsOnCooldown()) {
		UE_LOG(LogTemp, Warning, TEXT("Skill is on cooldown!"));
		return _keepCurrentState();
	}

	// The owner isn't forced to have a mana component. If it doesn't have one, it means that it can cast its skills for free.
	// Elements in the environment, like turrets that spit fire or clouds that spawn lightning bolts, are examples of this
	if (const auto manaC = pawn->FindComponentByClass<UManaComponent>()) {
		const double charMana = manaC->GetMana();
		const double manaCost = skill->GetManaCost();
		if (charMana < manaCost) {
			UE_LOG(LogTemp, Error, TEXT("Not enough mana to cast skill at index %i"), index);
			return _keepCurrentState();
		}

		manaC->SetMana(charMana - manaCost);
	}

	// TODO: if skill requires target return a targeting state, else return a casting state
	const auto castingState = NewObject<UCastingState>(controller, UCastingState::StaticClass());
	castingState->SetSkillInExecution(skill);
	return castingState;
}

TObjectPtr<USkillUserState> UIdleState::OnTick(float deltaTime, TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

void UIdleState::OnEnter(TObjectPtr<AController> controller) {
}

void UIdleState::OnLeave(TObjectPtr<AController> controller) {
}
