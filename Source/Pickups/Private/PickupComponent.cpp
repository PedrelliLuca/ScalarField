// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"

#include "InventoryInterface.h"

UPickupComponent::UPickupComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupComponent::InitializePickup(const TSubclassOf<UObject> itemClass, const int32 quantity) {
	check(itemClass && itemClass->ImplementsInterface(UItem::StaticClass()) && quantity > 0);
	_item = NewObject<UObject>(this, itemClass);
	_item->SetQuantity(quantity);
	_meshC->SetStaticMesh(_item->GetMesh());
	_interactableC->SetInteractableNameText(_item->GetNameText());
	_item->OnItemModified().AddUObject(this, &UPickupComponent::_onItemModified);

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

	const auto inventoryActorC = interactorController->GetPawn()->FindComponentByInterface(UInventory::StaticClass());
	check(IsValid(inventoryActorC));
	const TWeakInterfacePtr<IInventory> abstractInventory = Cast<IInventory>(inventoryActorC);

	const auto addResult = abstractInventory->TryAddItem(_item.GetInterface());
	if (addResult.Result != EItemAddResult::IAR_AllItemsAdded) {
		_item->SetQuantity(_item->GetQuantity() - addResult.ItemQuantityActuallyGiven);
	} else {
		GetOwner()->Destroy();
	}

	interactor->EndInteraction();
}
