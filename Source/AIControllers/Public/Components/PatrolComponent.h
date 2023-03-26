// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "PatrolComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AICONTROLLERS_API UPatrolComponent : public UActorComponent {
    GENERATED_BODY()

public:
    void StartPatrol();

    /** \brief Changes the current patrol objective to be the next one in the patrol sequence. If the end of the sequence has been
     * reached, the next element is the 0th. */
    void UpdatePatrolObjective();

    const FVector& GetCurrentPatrolObjective() const;

    float GetPatrolObjectiveTolerance() const { return _patrolObjectiveTolerance; }

private:
    /** \brief Sequence of vectors defining our patrol */
    UPROPERTY(EditInstanceOnly, Category = "Patrol")
    TArray<FVector> _patrolObjectives;

    /** \brief The maximum distance to any patrol objective that we consider acceptable */
    UPROPERTY(EditAnywhere, Category = "Patrol")
    float _patrolObjectiveTolerance = 150.0f;

    UPROPERTY(EditInstanceOnly, Category = "Patrol", meta = (ClampMin = "0"))
    int32 _startingPatrolObjectiveIdx = 0;

    UPROPERTY(VisibleAnywhere, Category = "Patrol", meta = (ClampMin = "0"))
    int32 _currentPatrolObjectiveIdx = 0;
};
