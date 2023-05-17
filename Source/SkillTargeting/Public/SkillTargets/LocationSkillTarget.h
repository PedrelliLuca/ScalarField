// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillTarget.h"

#include "LocationSkillTarget.generated.h"

UCLASS()
class SKILLTARGETING_API ULocationSkillTarget : public UObject, public ISkillTarget {
    GENERATED_BODY()

public:
    FVector GetTargetLocation() const override;

    void SetLocation(const FVector& location);

private:
    FVector _location{};
    bool _hasBeenSet = false;
};
