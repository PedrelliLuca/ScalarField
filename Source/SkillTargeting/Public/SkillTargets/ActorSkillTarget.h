// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SkillTarget.h"

#include "ActorSkillTarget.generated.h"

UCLASS()
class SKILLTARGETING_API UActorSkillTarget : public UObject, public ISkillTarget {
    GENERATED_BODY()

public:
    FVector GetTargetLocation() const override;

    TObjectPtr<AActor> GetActor() const { return _actor.Get(); }

private:
    TWeakObjectPtr<AActor> _actor;
};
