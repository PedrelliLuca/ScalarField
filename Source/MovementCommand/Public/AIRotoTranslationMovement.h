// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIMovementCommand.h"
#include "Parameters/RotoTranslationMovementParameters.h"

#include "AIRotoTranslationMovement.generated.h"

UCLASS(Blueprintable)
class MOVEMENTCOMMAND_API UAIRotoTranslationMovement : public UAIMovementCommand {
	GENERATED_BODY()

public:
	void OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination) final;
	void OnStopMovement(const TObjectPtr<AAIController>& aiController) final;
	void OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) final;

	void SetMovementParameters(const FMovementParameters& params) final;

private:
	void _onMovementCompleted(FAIRequestID requestId, const FPathFollowingResult& result);

	UPROPERTY(EditDefaultsOnly)
	FRotoTranslationMovementParameters _movementParameters;

	TWeakObjectPtr<UPathFollowingComponent> _ownerPathFollowingC = nullptr;
	FDelegateHandle _handleToMovementCompleted;
};