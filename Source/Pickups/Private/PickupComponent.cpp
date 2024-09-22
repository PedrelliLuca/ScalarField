// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupComponent.h"

#include "InventoryInterface.h"

UPickupComponent::UPickupComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UPickupComponent::InitializePickup(TSubclassOf<UObject> const itemClass, int32 const quantity) {
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

void UPickupComponent::_onTakePickup(TScriptInterface<IInteractor> const interactor) {
    TWeakObjectPtr<UActorComponent> const interactorC = Cast<UActorComponent>(interactor.GetObject());
    check(interactorC.IsValid());

    auto const interactorController = Cast<AController>(interactorC->GetOwner());
    check(IsValid(interactorController));

    auto const inventoryActorC = interactorController->GetPawn()->FindComponentByInterface(UInventory::StaticClass());
    check(IsValid(inventoryActorC));
    TWeakInterfacePtr<IInventory> const abstractInventory = Cast<IInventory>(inventoryActorC);

    auto const addResult = abstractInventory->TryAddItem(_item.GetInterface());
    if (addResult.Result != EItemAddResult::IAR_AllItemsAdded) {
        _item->SetQuantity(_item->GetQuantity() - addResult.ItemQuantityActuallyGiven);
    } else {
        GetOwner()->Destroy();
    }

    interactor->EndInteraction();
}
