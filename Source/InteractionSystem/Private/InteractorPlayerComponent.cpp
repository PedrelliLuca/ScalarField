// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractorPlayerComponent.h"

UInteractorPlayerComponent::UInteractorPlayerComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UInteractorPlayerComponent::PerformFocusCheck() {
    if (GetWorld()->GetRealTimeSeconds() - _interactionData.TimestampOfLastFocusCheck < _timeBetweenFocusChecks) {
        // Too little time has passed, let's save the line trace for later
        return;
    }

    check(_ownerPlayerController.IsValid());

    _interactionData.TimestampOfLastFocusCheck = GetWorld()->GetRealTimeSeconds();

    // Building the cursor line trace
    FVector cursorLoc{};
    FVector cursorDir{};
    _ownerPlayerController->DeprojectMousePositionToWorld(cursorLoc, cursorDir);
    auto const& traceStart = cursorLoc;
    auto const traceEnd = traceStart + cursorDir * INTERACTION_TRACE_LENGTH;

    // Did we find an actor blocking the visibility channel?
    FHitResult traceHit{};
    if (GetWorld()->LineTraceSingleByChannel(traceHit, traceStart, traceEnd, ECollisionChannel::ECC_Visibility) && traceHit.GetActor()) {
        // Does the actor we found have an interaction component?
        if (TWeakObjectPtr<UInteractableComponent> hitInteractableC = traceHit.GetActor()->FindComponentByClass<UInteractableComponent>();
            hitInteractableC.IsValid()) {
            auto const pawn = _ownerPlayerController->GetPawn();
            check(IsValid(pawn));

            // Is the component within reach?
            double const distance = (traceHit.ImpactPoint - pawn->GetActorLocation()).Size();
            if (distance <= hitInteractableC->GetInteractionDistance()) {
                // Is the component the one we're already focusing? If it isn't we update the interaction data,
                // otherwise we don't do anything
                if (hitInteractableC != _interactionData.InteractableBeingFocused) {
                    _endFocus();

                    // Here the actual replacement occurs
                    _interactionData.InteractableBeingFocused = hitInteractableC;
                    hitInteractableC->BeginFocus(this);
                }

                // This is crucial: we don't call _endFocus() if we're still focusing the same interactable.
                return;
            }
        }
    }

    _endFocus();
}

bool UInteractorPlayerComponent::PerformInteractionCheck() {
    // Are we pressing the interaction key while focusing on an interactable actor?
    if (!_getInteractableBeingFocused().IsValid()) {
        return false;
    }

    // Are we already interacting with the focused actor?
    if (_getInteractableBeingFocused() == _getInteractableBeingInteracted()) {
        return false;
    }

    EndInteraction();
    _interactionData.InteractableBeingInteracted = _getInteractableBeingFocused();

    _getInteractableBeingInteracted()->BeginInteraction(this);

    if (FMath::IsNearlyZero(_getInteractableBeingInteracted()->GetInteractionTime())) {
        UE_LOG(LogTemp, Warning, TEXT("Instantenous interactions ignore tactical pause!!"));
        _interact();
    } else {
        GetWorld()->GetTimerManager().SetTimer(
            _interactionTimerHandle, this, &UInteractorPlayerComponent::_interact, _getInteractableBeingInteracted()->GetInteractionTime());
    }

    return true;
}

void UInteractorPlayerComponent::EndInteraction() {
    GetWorld()->GetTimerManager().ClearTimer(_interactionTimerHandle);

    if (_getInteractableBeingInteracted().IsValid()) {
        _getInteractableBeingInteracted()->EndInteraction(this);
    }

    _interactionData.InteractableBeingInteracted = nullptr;
}

bool UInteractorPlayerComponent::IsInteracting() const {
    return _getInteractableBeingInteracted().IsValid();
}

double UInteractorPlayerComponent::GetTimeLeftBeforeInteraction() const {
    return GetWorld()->GetTimerManager().GetTimerRemaining(_interactionTimerHandle);
}

void UInteractorPlayerComponent::BeginPlay() {
    Super::BeginPlay();

    _ownerPlayerController = Cast<APlayerController>(GetOwner());
    check(_ownerPlayerController.IsValid());
}

void UInteractorPlayerComponent::_endFocus() {
    // Were we focusing something? If so, we call EndFocus on it, as we're no longer focusing it.
    if (_getInteractableBeingFocused().IsValid()) {
        _getInteractableBeingFocused()->EndFocus(this);
    }

    // Now there's nothing we're focusing, our data must reflect that.
    _interactionData.InteractableBeingFocused = nullptr;
}

void UInteractorPlayerComponent::_interact() {
    GetWorld()->GetTimerManager().ClearTimer(_interactionTimerHandle);

    check(_getInteractableBeingInteracted().IsValid());
    _getInteractableBeingInteracted()->Interact(this);
}
