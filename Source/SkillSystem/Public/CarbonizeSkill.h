// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbstractSkill.h"
#include "CoreMinimal.h"

#include "CarbonizeSkill.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SKILLSYSTEM_API UCarbonizeSkill : public UAbstractSkill {
    GENERATED_BODY()

public:
    void ExecuteCast() override;

    bool IsValidTarget(int32 targetIndex, TObjectPtr<AActor> target) const override;

#if DO_CHECK
    void CheckParametersSanity() const override;
#endif

private:
    UPROPERTY(EditAnywhere, Category = "CarbonizeParameters")
    double _carbonizationTemperature = 900.;
};
