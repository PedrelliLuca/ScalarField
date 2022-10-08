// Fill out your copyright notice in the Description page of Project Settings.

#include "Skills/IceWallSkill.h"

void UIceWallSkill::Cast() {
	Super::Cast();

	UE_LOG(LogTemp, Warning, TEXT("UIceWallSkill::Cast()"));
}
