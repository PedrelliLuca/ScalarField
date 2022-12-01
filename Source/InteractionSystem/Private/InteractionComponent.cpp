// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

UInteractionComponent::UInteractionComponent() {
	SetComponentTickEnabled(false);

	_interactionTime = 0.0;
	_interactionDistance = 200.0;
	_interactableNameText = FText::FromString(TEXT("Interactable Object"));
	_interactableActionText = FText::FromString(TEXT("Interact"));

	// >>>>> UWidgetComponent setup
	Space = EWidgetSpace::Screen;
	DrawSize = FIntPoint(600, 100);
	bDrawAtDesiredSize = true;

	SetActive(true);
	// Don't show me the UI by default, I want it to appear only when the player is focusing the owner actor
	SetHiddenInGame(true);
	// <<<<<
}

void UInteractionComponent::BeginFocus(TWeakObjectPtr<AController> interactingController) {
	SetHiddenInGame(false);
	_onBeginFocus.Broadcast(MoveTemp(interactingController));
}

void UInteractionComponent::EndFocus(TWeakObjectPtr<AController> interactingController) {
	SetHiddenInGame(true);
	_onEndFocus.Broadcast(MoveTemp(interactingController));
}

void UInteractionComponent::BeginInteraction(TWeakObjectPtr<AController> interactingCharacter) {
	_onBeginInteraction.Broadcast(MoveTemp(interactingCharacter));
}

void UInteractionComponent::Interact(TWeakObjectPtr<AController> interactingCharacter) {
	_onInteraction.Broadcast(MoveTemp(interactingCharacter));
}

void UInteractionComponent::EndInteraction(TWeakObjectPtr<AController> interactingCharacter) {
	_onEndInteraction.Broadcast(MoveTemp(interactingCharacter));
}

void UInteractionComponent::BeginPlay() {
	Super::BeginPlay();

	/* Enable the outline for each meshComponent that is visible. We want to show the player that the owner actor is
	 * interactable. */
	TInlineComponentArray<UMeshComponent*> meshComponents;
	GetOwner()->GetComponents<UMeshComponent>(meshComponents);
	for (const auto meshC : meshComponents) {
		if (meshC->IsVisible()) {
			meshC->SetRenderCustomDepth(true);
		}
	}
}
