// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManipulationSubsystem.h"

void UInventoryManipulationSubsystem::Initialize(FSubsystemCollectionBase& collection) {
	Super::Initialize(collection);

	_inventoryToggleController = NewObject<UInventoryToggleController>(this, UInventoryToggleController::StaticClass());
}

void UInventoryManipulationSubsystem::SetInventoryContainerWidget(TWeakInterfacePtr<IInventoryContainerWidget> inventoryContainer) {
	_inventoryToggleController->SetInventoryContainerWidget(MoveTemp(inventoryContainer));
}

void UInventoryManipulationSubsystem::SetHUDToShowOnClose(TWeakInterfacePtr<IPawnBindableWidget> widgetOnClose) {
	_inventoryToggleController->SetHUDToShowOnClose(MoveTemp(widgetOnClose));
}

void UInventoryManipulationSubsystem::OpenInventoryOfActor(TWeakObjectPtr<AActor> actor) {
	_inventoryToggleController->OpenInventoryOfActor(MoveTemp(actor));
}

void UInventoryManipulationSubsystem::CloseInventory() {
	_inventoryToggleController->CloseInventory();
}
