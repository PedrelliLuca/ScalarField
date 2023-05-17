// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillTarget.h"

#include "LocationSkillTarget.generated.h"

UCLASS()
class SKILLTARGETING_API ULocationSkillTarget : public UObject, public ISkillTarget {
    GENERATED_BODY()

public:
    void Init(const FSkillTargetPacket& targetPacket) override;
    
    FVector GetTargetLocation() const override;

private:
    FVector _location{};
    bool _hasBeenSet = false;
};
