// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryToggleController.h"

#include "InventoryInterface.h"

void UInventoryToggleController::SetInventoryContainerWidget(TWeakInterfacePtr<IInventoryContainerWidget> inventoryPresenter) {
	check(inventoryPresenter.IsValid());
	_inventoryContainer = MoveTemp(inventoryPresenter);
	_inventoryContainer->OnCloseFromUI().AddUObject(this, &UInventoryToggleController::_inventoryWidgetClosedCallback);
}

void UInventoryToggleController::SetHUDToShowOnClose(TWeakInterfacePtr<IPawnBindableWidget>&& widgetOnClose) {
	check(widgetOnClose.IsValid());
	_hudToShowOnClose = MoveTemp(widgetOnClose);
}

void UInventoryToggleController::OpenInventoryOfActor(TWeakObjectPtr<AActor>&& actor) {
	check(_hudToShowOnClose.IsValid());
	_hudToShowOnClose->Hide();
	_hudToShowOnClose->UnbindCurrentPawn();
	
	const auto inventoryC = actor->FindComponentByInterface(UInventory::StaticClass());
	TWeakInterfacePtr<IInventory> inventory = Cast<IInventory>(inventoryC);
	check(inventory.IsValid() && _inventoryContainer.IsValid());
	_inventoryContainer->ShowInventory(MoveTemp(inventory));
}

void UInventoryToggleController::CloseInventory() {
	check(_inventoryContainer.IsValid());
	_inventoryContainer->HideInventory();
	
	check(_hudToShowOnClose.IsValid());
	_hudToShowOnClose->BindCurrentPawn();
	_hudToShowOnClose->Show();
}

void UInventoryToggleController::_inventoryWidgetClosedCallback() {
	_onInventoryClosedFromUI.Broadcast();
}
