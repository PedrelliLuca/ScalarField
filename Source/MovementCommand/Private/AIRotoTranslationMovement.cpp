// Fill out your copyright notice in the Description page of Project Settings.

#include "AIRotoTranslationMovement.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"

#define LOCTEXT_NAMESPACE "AIRotoTranslationMovement"

void UAIRotoTranslationMovement::OnSetDestination(const FVector& destination) {
    check(_aiController.IsValid());

    // Always enter if you can't ever ignore OnSetDestination() calls. If you can, then enter only if you're not already following a path.
    if (!_movementParameters.IgnoreNewDestinationsIfMoving || !_aiController->IsFollowingAPath()) {
        // Enter if the location we're trying to set isn't identical to the already set one.
        if (!_aiController->GetPathFollowingComponent()->GetPathDestination().Equals(destination)) {
            // NOTE: this function is a copy of UAIBlueprintHelperLibrary::SimpleMoveToLocation. Why not using the original then? Because I need to inject
            // the _movementParameters such as the AcceptanceRadius.
            _moveToLocation(_aiController.Get(), destination);
        }
    }
}

void UAIRotoTranslationMovement::OnStopMovement() {
    check(_aiController.IsValid());
    _aiController->StopMovement();
}

void UAIRotoTranslationMovement::OnMovementTick(float deltaTime) {
}

void UAIRotoTranslationMovement::SetMovementParameters(const FMovementParameters& params) {
    _movementParameters = params.RotoTranslationMovementParameters;
}

bool UAIRotoTranslationMovement::IsMoving() const {
    return IsValid(_aiController->GetPathFollowingComponent()) && _aiController->IsFollowingAPath();
}

UPathFollowingComponent* UAIRotoTranslationMovement::_initNavigationControl(AController& Controller) {
    AAIController* AsAIController = Cast<AAIController>(&Controller);
    UPathFollowingComponent* PathFollowingComp = nullptr;

    if (AsAIController) {
        PathFollowingComp = AsAIController->GetPathFollowingComponent();
    } else {
        PathFollowingComp = Controller.FindComponentByClass<UPathFollowingComponent>();
        if (PathFollowingComp == nullptr) {
            PathFollowingComp = NewObject<UPathFollowingComponent>(&Controller);
            PathFollowingComp->RegisterComponentWithWorld(Controller.GetWorld());
            PathFollowingComp->Initialize();
        }
    }

    return PathFollowingComp;
}

void UAIRotoTranslationMovement::_moveToLocation(AController* Controller, const FVector& GoalLocation) {
    UNavigationSystemV1* NavSys = Controller ? FNavigationSystem::GetCurrent<UNavigationSystemV1>(Controller->GetWorld()) : nullptr;
    if (NavSys == nullptr || Controller == nullptr || Controller->GetPawn() == nullptr) {
        UE_LOG(LogNavigation, Warning,
            TEXT("UNavigationSystemV1::SimpleMoveToActor called for NavSys:%s Controller:%s controlling Pawn:%s (if any of these is None then there's your "
                 "problem"),
            *GetNameSafe(NavSys), *GetNameSafe(Controller), Controller ? *GetNameSafe(Controller->GetPawn()) : TEXT("NULL"));
        return;
    }

    UPathFollowingComponent* PFollowComp = _initNavigationControl(*Controller);

    if (PFollowComp == nullptr) {
        FMessageLog("PIE").Warning(
            FText::Format(LOCTEXT("SimpleMoveErrorNoComp", "SimpleMove failed for {0}: missing components"), FText::FromName(Controller->GetFName())));
        return;
    }

    if (!PFollowComp->IsPathFollowingAllowed()) {
        FMessageLog("PIE").Warning(
            FText::Format(LOCTEXT("SimpleMoveErrorMovement", "SimpleMove failed for {0}: movement not allowed"), FText::FromName(Controller->GetFName())));
        return;
    }

    const bool bAlreadyAtGoal = PFollowComp->HasReached(
        GoalLocation, EPathFollowingReachMode::OverlapAgent, _movementParameters.AcceptanceRadius); // My addition: injection of acceptance radius

    // script source, keep only one move request at time
    if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle) {
        PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest, FAIRequestID::AnyRequest,
            bAlreadyAtGoal ? EPathFollowingVelocityMode::Reset : EPathFollowingVelocityMode::Keep);
    }

    // script source, keep only one move request at time
    if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle) {
        PFollowComp->AbortMove(*NavSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
    }

    if (bAlreadyAtGoal) {
        PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Success);
    } else {
        const FVector AgentNavLocation = Controller->GetNavAgentLocation();
        const ANavigationData* NavData = NavSys->GetNavDataForProps(Controller->GetNavAgentPropertiesRef(), AgentNavLocation);
        if (NavData) {
            FPathFindingQuery Query(Controller, *NavData, AgentNavLocation, GoalLocation);
            FPathFindingResult Result = NavSys->FindPathSync(Query);
            if (Result.IsSuccessful()) {
                auto MoveReq = FAIMoveRequest(GoalLocation);
                MoveReq.SetAcceptanceRadius(_movementParameters.AcceptanceRadius); // My addition: injection of acceptance radius
                MoveReq.SetAllowPartialPath(_movementParameters.AllowPartialPath); // My addition: injection of allow partial path

                PFollowComp->RequestMove(MoveTemp(MoveReq), Result.Path);
            } else if (PFollowComp->GetStatus() != EPathFollowingStatus::Idle) {
                PFollowComp->RequestMoveWithImmediateFinish(EPathFollowingResult::Invalid);
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE
