// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractSkill.h"
#include "CoreMinimal.h"

#include "IceWallSkill.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UIceWallSkill : public UAbstractSkill {
    GENERATED_BODY()

public:
    void ExecuteCast() override;

#if DO_CHECK
    void CheckParametersSanity() const override;
#endif
};
