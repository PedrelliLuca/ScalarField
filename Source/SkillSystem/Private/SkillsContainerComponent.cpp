// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillsContainerComponent.h"

USkillsContainerComponent::USkillsContainerComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillsContainerComponent::BeginPlay() {
	Super::BeginPlay();


	int32 j = 0;
	for (const auto skillParametersClass : _skillsParametersClasses) {
		const auto parameters = NewObject<UNewSkillParameters>(this, skillParametersClass);
		
		// Create concrete skill and pass parameters to it so that it can inti itself
		// How do I understand which skill to spawn?
		// Option A: it's a parameter in the UParametersTest class
		// Option B: i find a way to setup a pair of callbacks from the UParametersTest, callback 1: function to call when CastSkill() is called, callback 2: function to call when InitSkill() is called

		int32 i = 0;
		for (const auto capsuleSpawner : parameters->CapsuleSpawnerParameters) {
			// Create capsule spawner component with given attributes and add it to the UAbstractSkill
			UE_LOG(LogTemp, Warning, TEXT("Component #%i Capsule #%i Half Height: %.2f"), j + 1, i + 1, capsuleSpawner.HalfHeight);
			++i;
		}

		// And same for the arms, the particles, the actor spawners, ...
		++j;
	}
}