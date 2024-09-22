// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PatrolComponent.h"

void UPatrolComponent::StartPatrol() {
    _currentPatrolObjectiveIdx = _startingPatrolObjectiveIdx;
}

void UPatrolComponent::UpdatePatrolObjective() {
    _currentPatrolObjectiveIdx = (_currentPatrolObjectiveIdx + 1) % _patrolObjectives.Num();
}

FVector const& UPatrolComponent::GetCurrentPatrolObjective() const {
    return _patrolObjectives[_currentPatrolObjectiveIdx];
}
