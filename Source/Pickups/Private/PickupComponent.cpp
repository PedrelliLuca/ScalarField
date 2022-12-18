// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"

#include "InventoryComponent.h"

UPickupComponent::UPickupComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UPickupComponent::InitializePickup() {
	if (!IsValid(_itemTemplate)) {
		return;
	}

	check(_itemTemplate->GetClass() && _itemTemplate->GetQuantity() > 0);
	_item = NewObject<UInventoryItem>(this, _itemTemplate->GetClass());
	_item->SetQuantity(_itemTemplate->GetQuantity());
	_meshC->SetStaticMesh(_item->GetMesh());
	_interactableC->SetInteractableNameText(_item->GetNameText());
	_item->OnItemModified().AddDynamic(this, &UPickupComponent::_onItemModified);

	_interactableC->RefreshWidget();
}

#if WITH_EDITOR
void UPickupComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
	Super::PostEditChangeProperty(propertyChangedEvent);

	const auto property = propertyChangedEvent.Property;
	const auto propertyName = property != nullptr ? property->GetFName() : NAME_None;

	// If a new pickup is selected in the property editor, change the mesh to reflect the new item being selected
	if (propertyName == GET_MEMBER_NAME_CHECKED(UPickupComponent, _itemTemplate)) {
		// const auto initTempProperty = CastFieldChecked<FObjectPtrProperty>(property);
		const auto meshC = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
		check(IsValid(meshC));
		if (IsValid(_itemTemplate)) {
			meshC->SetStaticMesh(_itemTemplate->GetMesh());
		}
	}
}
#endif

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
