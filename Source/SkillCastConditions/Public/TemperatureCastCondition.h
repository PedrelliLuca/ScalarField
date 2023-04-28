// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCastConditionInterface.h"
#include "ThermodynamicComponent.h"

#include "TemperatureCastCondition.generated.h"

/** \brief Ways to compare the subject's temperature with the condition's temperature. E.g., "Less Than" means that the subject's temperature will have to
 * be smaller than this condition's temperature. */
UENUM(BlueprintType)
enum class ETemperatureComparisonStyle : uint8
{
    TCS_None UMETA(DisplayName = "None"),
    TCS_Equality UMETA(DisplayName = "Equality"),
    TCS_LessThan UMETA(DisplayName = "Less Than"),
    TCS_GreaterThan UMETA(DisplayName = "Greater Than")
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

    /** \brief Determines how the subject's temperature will be compared with _castConditionTemperature */
    UPROPERTY(EditAnywhere)
    ETemperatureComparisonStyle _temperatureComparison = ETemperatureComparisonStyle::TCS_None;

private:
    TWeakObjectPtr<AActor> _conditionSubject;
    TWeakObjectPtr<UThermodynamicComponent> _subjectThermoC;
};
