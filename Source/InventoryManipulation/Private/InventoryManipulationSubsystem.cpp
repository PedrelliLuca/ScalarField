// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManipulationSubsystem.h"

void UInventoryManipulationSubsystem::Initialize(FSubsystemCollectionBase& collection) {
	Super::Initialize(collection);

	_inventoryToggleController = NewObject<UInventoryToggleController>(this, UInventoryToggleController::StaticClass());
	_itemUsageController = NewObject<UItemUsageController>(this, UItemUsageController::StaticClass());
}

void UInventoryManipulationSubsystem::SetInventoryContainerWidget(TWeakInterfacePtr<IInventoryContainerWidget> inventoryContainer) {
	_inventoryToggleController->SetInventoryContainerWidget(inventoryContainer);
	_itemUsageController->SetItemContainerWidget(inventoryContainer->GetInventoryWidget());
}

void UInventoryManipulationSubsystem::SetHUDToShowOnClose(TWeakInterfacePtr<IPawnBindableWidget> widgetOnClose) {
	_inventoryToggleController->SetHUDToShowOnClose(MoveTemp(widgetOnClose));
}

void UInventoryManipulationSubsystem::OpenInventoryOfActor(TWeakObjectPtr<AActor> actor) {
	_inventoryToggleController->OpenInventoryOfActor(MoveTemp(actor));
	_itemUsageController->BindItemUsage();
}

void UInventoryManipulationSubsystem::CloseInventory() {
	_inventoryToggleController->CloseInventory();
	_itemUsageController->UnbindItemUsage();
}
