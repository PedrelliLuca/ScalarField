// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "SkillCastCondition.h"

#include "HealthCastCondition.generated.h"

/** \brief Ways to compare the subject's health with the condition's health value. E.g., "Less Than" means that the subject's health will have to
 * be smaller than this condition's value. */
UENUM(BlueprintType)
enum class EHealthComparisonStyle : uint8
{
    HCS_None UMETA(DisplayName = "None"),
    HCS_LessThan UMETA(DisplayName = "Less Than"),
    HCS_GreaterThan UMETA(DisplayName = "Greater Than")
};

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class SKILLCASTCONDITIONS_API UHealthCastCondition : public USkillCastCondition {
    GENERATED_BODY()

public:
    bool IsVerified() const override;

    void SetConditionSubject(TObjectPtr<AActor> subject) override;

protected:
    /** \brief The temperature for which we want to create a cast condition */
    UPROPERTY(EditAnywhere)
    double _castConditionHealth = 100.0;

    /** \brief Determines how the subject's temperature will be compared with _castConditionTemperature */
    UPROPERTY(EditAnywhere)
    EHealthComparisonStyle _healthComparison = EHealthComparisonStyle::HCS_None;

private:
    TWeakObjectPtr<AActor> _conditionSubject;
    TWeakObjectPtr<UHealthComponent> _subjectHealthC;
};
