// Fill out your copyright notice in the Description page of Project Settings.

#include "AIRotoTranslationMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"

void UAIRotoTranslationMovement::OnSetDestination(const TObjectPtr<AAIController>& aiController, const FVector& destination) {
	// TODO: this shouldn't be here OR, at the very least, we should check that the destination of the path being followed is not the same we have as input.
	// This can be done via UPathFollowingComponent::GetPathDestination()
	if (aiController->IsFollowingAPath()) {
		return;
	}

	// The following code is an edit of UAIBlueprintHelperLibrary::SimpleMoveToLocation().

	UNavigationSystemV1* navSys = aiController ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(aiController->GetWorld()) : nullptr;
	if (navSys == nullptr || aiController == nullptr || aiController->GetPawn() == nullptr) {
		UE_LOG(LogNavigation, Warning, TEXT("UNavigationSystemV1::SimpleMoveToActor called for NavSys:%s Controller:%s controlling Pawn:%s (if any of these is None then there's your problem"),
			*GetNameSafe(navSys), *GetNameSafe(aiController), aiController ? *GetNameSafe(aiController->GetPawn()) : TEXT("NULL"));
		_setIsMoving(false);
		return;
	}

	_ownerPathFollowingC = aiController->GetPathFollowingComponent();
	check(_ownerPathFollowingC.IsValid());

	if (!_ownerPathFollowingC->IsPathFollowingAllowed()) {
		_setIsMoving(false);
		return;
	}

	const bool bAlreadyAtGoal = _ownerPathFollowingC->HasReached(destination, EPathFollowingReachMode::OverlapAgent);

	// script source, keep only one move request at time
	if (_ownerPathFollowingC->GetStatus() != EPathFollowingStatus::Idle) {
		_ownerPathFollowingC->AbortMove(*navSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
			, FAIRequestID::AnyRequest, bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
	}
	// script source, keep only one move request at time
	if (_ownerPathFollowingC->GetStatus() != EPathFollowingStatus::Idle) {
		_ownerPathFollowingC->AbortMove(*navSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
	}

	if (bAlreadyAtGoal) {
		_ownerPathFollowingC->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
		_setIsMoving(false);
		return;
	}

	const FVector agentNavLocation = aiController->GetNavAgentLocation();
	const ANavigationData* navData = navSys->GetNavDataForProps(aiController->GetNavAgentPropertiesRef(), agentNavLocation);
	if (navData) {
		FPathFindingQuery query(aiController, *navData, agentNavLocation, destination);
		FPathFindingResult result = navSys->FindPathSync(query);
		if (result.IsSuccessful()) {

			// See UBTTask_MoveTo for a complete customization.
			FAIMoveRequest moveReq;
			moveReq.SetGoalLocation(destination);
			moveReq.SetAllowPartialPath(_allowPartialPath);
			moveReq.SetAcceptanceRadius(_acceptanceRadius);

			// Found a path
			_ownerPathFollowingC->RequestMove(moveReq, result.Path);
			_setIsMoving(true);

			_handleToMovementCompleted = _ownerPathFollowingC->OnRequestFinished.AddUObject(this, &UAIRotoTranslationMovement::_onMovementCompleted);
		} else {
			// Didn't find a path
			if (_ownerPathFollowingC->GetStatus() != EPathFollowingStatus::Idle) {
				_ownerPathFollowingC->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
			}
			_setIsMoving(false);
		}
	} else {
		// I think that a null navData means that it's impossible to retrieve the nav mesh / nav graph
		_setIsMoving(false);
	}
;}

void UAIRotoTranslationMovement::OnStopMovement(const TObjectPtr<AAIController>& aiController) {
	aiController->StopMovement();
	_setIsMoving(false);
}

void UAIRotoTranslationMovement::OnMovementTick(const TObjectPtr<AAIController>& aiController, float deltaTime) {
	// Do we even need this for AI?
}

void UAIRotoTranslationMovement::_onMovementCompleted(FAIRequestID requestId, const FPathFollowingResult& result) {
	if (_ownerPathFollowingC.IsValid()) {
		_ownerPathFollowingC->OnRequestFinished.Remove(_handleToMovementCompleted);
		_ownerPathFollowingC = nullptr;
	}
	_setIsMoving(false);
}
