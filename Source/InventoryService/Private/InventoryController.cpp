// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryController.h"

#include "InventoryComponent.h"
#include "TacticalPauseWorldSubsystem.h"

void UInventoryController::SetWidgetsPresenter(TWeakObjectPtr<UWidgetsPresenterComponent>&& widgetsPresenterC) {
	check(widgetsPresenterC.IsValid());
	_widgetsPresenterC = MoveTemp(widgetsPresenterC);
	// _widgetsPresenterC->GetInventoryPresenterWidget()->OnButtonClose().AddUObject(this, &UInventoryController::_onInventoryClose);

}

void UInventoryController::ShowInventoryOfActor(TWeakObjectPtr<AActor>&& actor) {
	check(actor.IsValid());
	check(_widgetsPresenterC.IsValid());

	const TWeakObjectPtr<UInventoryComponent> inventory = actor->FindComponentByClass<UInventoryComponent>();
	
	check(inventory.IsValid());

	// _widgetsPresenterC->ShowInventory(inventory);

	// _widgetsPresenterC->GetInventoryPresenterWidget()->GetInventoryWidget()->OnItemFromInventoryBeingUsed().AddUObject(this, &UInventoryController::_useItemOfInventory);
}

void UInventoryController::_onInventoryClose() {
	_onInventoryWidgetClosed.Broadcast();
}

void UInventoryController::_useItemOfInventory(TWeakObjectPtr<UInventoryItem> item, TWeakObjectPtr<UInventoryComponent> inventory) {
	const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
	pauseSubsys->OnTacticalPauseToggle().Remove(_itemUsageOnPauseToggleHandle);
	if (!pauseSubsys->IsTacticalPauseOn()) {
		inventory->UseItem(item);
		return;
	}

	// The tactical pause is one, we can't use the item yet. We predispose the following lambda to be called once the
	// tactical pause is toggled off.
	
	_itemUsageOnPauseToggleHandle = pauseSubsys->OnTacticalPauseToggle().AddLambda([this, item, inventory](const bool bIsTacticalPauseOn, const double currentWorldTimeDilation) {
		// This must have been called when the tactical pause has been turned off
		check(!bIsTacticalPauseOn);

		// I don't see how the UInventoryController could not be valid, its outer is the subsystem...
		check(IsValid(this));

		// We were in tactical pause when this lambda was created, meaning that from the creation and call we do not
		// expect anything to be changed on the inventory owner, whoever it is. Therefore, its inventory and the item
		// in it must still be valid.
		check(item.IsValid())
		check(inventory.IsValid());
		
		_useItemOfInventory(item, inventory);
	});
}
