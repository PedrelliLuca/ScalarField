// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"

#include "InventoryComponent.h"

UPickupComponent::UPickupComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupComponent::InitializePickup(const TSubclassOf<UInventoryItem> itemClass, const int32 quantity) {

	check(itemClass && quantity > 0);
	_item = NewObject<UInventoryItem>(this, itemClass);
	_item->SetQuantity(quantity);
	_meshC->SetStaticMesh(_item->GetMesh());
	_interactableC->SetInteractableNameText(_item->GetNameText());
	_item->OnItemModified().AddDynamic(this, &UPickupComponent::_onItemModified);

	_interactableC->RefreshWidget();
}

void UPickupComponent::BeginPlay() {
	Super::BeginPlay();
	_interactableC = GetOwner()->FindComponentByClass<UInteractableComponent>();
	check(_interactableC.IsValid());
	_interactableC->OnInteraction().AddDynamic(this, &UPickupComponent::_onTakePickup);
	_meshC = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	check(_meshC.IsValid());
}

void UPickupComponent::_onItemModified() {
	check(_interactableC.IsValid());
	_interactableC->RefreshWidget();
}

void UPickupComponent::_onTakePickup(const TScriptInterface<IInteractor> interactor) {
	const TWeakObjectPtr<UActorComponent> interactorC = Cast<UActorComponent>(interactor.GetObject());
	check(interactorC.IsValid());

	const auto interactorController = Cast<AController>(interactorC->GetOwner());
	check(IsValid(interactorController));

	const TWeakObjectPtr<UInventoryComponent> pawnInventoryC = interactorController->GetPawn()->FindComponentByClass<UInventoryComponent>();
	check(pawnInventoryC.IsValid());

	const auto addResult = pawnInventoryC->TryAddItem(_item);
	if (addResult.Result != EItemAddResult::IAR_AllItemsAdded) {
		_item->SetQuantity(_item->GetQuantity() - addResult.ItemQuantityActuallyGiven);
	} else {
		GetOwner()->Destroy();
	}

	interactor->EndInteraction();
}
