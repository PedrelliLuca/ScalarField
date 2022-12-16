// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryController.h"

#include "InventoryComponent.h"

void UInventoryController::SetWidgetsPresenter(TWeakObjectPtr<UWidgetsPresenterComponent>&& widgetsPresenterC) {
	check(widgetsPresenterC.IsValid());
	_widgetsPresenterC = MoveTemp(widgetsPresenterC);
	_widgetsPresenterC->GetInventoryPresenterWidget()->OnButtonClose().AddUObject(this, &UInventoryController::_onInventoryClose);

}

void UInventoryController::ShowInventoryOfActor(TWeakObjectPtr<AActor>&& actor) {
	check(actor.IsValid());
	check(_widgetsPresenterC.IsValid());

	const TWeakObjectPtr<UInventoryComponent> inventory = actor->FindComponentByClass<UInventoryComponent>();
	
	check(inventory.IsValid());

	_widgetsPresenterC->ShowInventory(inventory);
}

void UInventoryController::_onInventoryClose() {
	_onInventoryWidgetClosed.Broadcast();
}
