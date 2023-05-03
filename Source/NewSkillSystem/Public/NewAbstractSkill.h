// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCastResult.h"

#include "NewAbstractSkill.generated.h"

/**
 *
 */
UCLASS(NotBlueprintable, Abstract, DefaultToInstanced)
class NEWSKILLSYSTEM_API UNewAbstractSkill : public UObject {
    GENERATED_BODY()

public:
    FSkillCastResult TryCast();
};
