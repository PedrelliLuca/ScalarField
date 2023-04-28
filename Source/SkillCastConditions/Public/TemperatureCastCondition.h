// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCastConditionInterface.h"

#include "TemperatureCastCondition.generated.h"

/** \brief Ways to compare the skill owner's temperature with the condition's temperature. */
UENUM(BlueprintType)
enum class ETemperatureComparison : uint8
{
    TC_None UMETA(DisplayName = "None"),
    TC_Equal UMETA(DisplayName = "Equal"),
    TC_LessThan UMETA(DisplayName = "Less Than"),
    TC_GreaterThan UMETA(DisplayName = "Greater Than")
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class SKILLCASTCONDITIONS_API UTemperatureCastCondition : public UObject, public ISkillCastCondition {
    GENERATED_BODY()

public:
    bool IsVerified() const override;

    void SetConditionSubject(TObjectPtr<AActor> subject) override;

protected:
    /** \brief The temperature for which we want to create a cast condition */
    UPROPERTY(EditAnywhere)
    double _castConditionTemperature = 300.0;

    /** \brief Determines how the skill owner's temperature will be compared with _castConditionTemperature */
    UPROPERTY(EditAnywhere)
    ETemperatureComparison _temperatureComparison = ETemperatureComparison::TC_None;

private:
    TWeakObjectPtr<AActor> _conditionSubject;
};
