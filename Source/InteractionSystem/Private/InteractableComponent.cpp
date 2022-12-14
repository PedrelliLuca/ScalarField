// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"

#include "InteractionWidget.h"

UInteractableComponent::UInteractableComponent() {
	SetComponentTickEnabled(false);

	_interactionTime = 0.0;
	_interactionDistance = 200.0;
	_bAllowMultipleInteractors = false;
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

void UInteractableComponent::BeginFocus(TScriptInterface<IInteractor> interactor) {
	if (!IsActive() || !IsValid(interactor.GetObject())) {
		return;
	}
	
	SetHiddenInGame(false);
	RefreshWidget();
	_onBeginFocus.Broadcast(MoveTemp(interactor));
}

void UInteractableComponent::EndFocus(TScriptInterface<IInteractor> interactor) {
	SetHiddenInGame(true);
	_onEndFocus.Broadcast(MoveTemp(interactor));
}

void UInteractableComponent::BeginInteraction(TScriptInterface<IInteractor> interactor) {
	if (_canInteract(interactor)) {
		_interactors.Emplace(interactor);
		_onBeginInteraction.Broadcast(MoveTemp(interactor));
	}
}

void UInteractableComponent::Interact(TScriptInterface<IInteractor> interactor) {
	_onInteraction.Broadcast(MoveTemp(interactor));
}

void UInteractableComponent::EndInteraction(TScriptInterface<IInteractor> interactor) {
	_interactors.Remove(interactor);
	_onEndInteraction.Broadcast(MoveTemp(interactor));
}

double UInteractableComponent::GetInteractionPercentage() const {
	// Show the interaction percentage of the interactor that has interacted for the longest time.
	// This could be refactored in the future so that an array of percentages is returned, one for each interactor,
	// with the player's one being marked. This way, the widget could show a progress bar for each interactor
	double highestPercentage = 0.0;
	for (const auto& interactor : _interactors) {
		const double interactorPercentage = 1.0 - interactor->GetTimeLeftBeforeInteraction() / _interactionTime;
		if (interactorPercentage > highestPercentage) {
			highestPercentage = interactorPercentage;
		}
	}
	return highestPercentage; 
}

void UInteractableComponent::SetInteractableNameText(const FText& newInteractableNameText)  {
	_interactableNameText = newInteractableNameText;
	RefreshWidget();
}

void UInteractableComponent::SetInteractableActionText(const FText& newInteractableActionText) {
	_interactableActionText = newInteractableActionText;
	RefreshWidget();
}

void UInteractableComponent::RefreshWidget() {
	if (bHiddenInGame) {
		// There is no point in updating the widget if it's hidden...
		return;
	}

	const TWeakObjectPtr<UInteractionWidget> interactionWidget = Cast<UInteractionWidget>(GetUserWidgetObject());
	// The widget associated to an interaction component has to be an interaction widget
	check(interactionWidget.IsValid());

	interactionWidget->UpdateInteractionWidget(this);
}

void UInteractableComponent::BeginPlay() {
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

void UInteractableComponent::Deactivate() {
	Super::Deactivate();
	
	for (const auto& interactor : _interactors) {
		if (IsValid(interactor.GetObject())) {
			EndFocus(interactor);
			EndInteraction(interactor);
		}
	}
 	_interactors.Empty();
}

bool UInteractableComponent::_canInteract(const TScriptInterface<IInteractor>& interactor) const {
	// We want to stop a 2nd interactor from interacting in case multiple interactors aren't allowed.
	const bool bIsAlreadyBeingInteracted = !_bAllowMultipleInteractors && _interactors.Num() >= 1;
	// Moreover, the interaction cannot occur if the interactor isn't valid or this component isn't active.
	return IsValid(interactor.GetObject()) && IsActive() && !bIsAlreadyBeingInteracted;
}
