// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPresenterWidget.h"

#include "Components/OverlaySlot.h"

void UInventoryPresenterWidget::ShowInventory(TWeakInterfacePtr<IInventory> inventory) {
	AddToViewport();
	_inventoryWidget->SetInventory(MoveTemp(inventory));
	_itemFromInventoryDiscardedHandle = _inventoryWidget->OnItemFromInventoryDiscarded().AddUObject(this, &UInventoryPresenterWidget::_onItemFromInventoryBeingDiscarded);
}

void UInventoryPresenterWidget::HideInventory() {
	RemoveFromParent();

	if (_itemFromInventoryDiscardedHandle.IsValid()) {
		_inventoryWidget->OnItemFromInventoryDiscarded().Remove(_itemFromInventoryDiscardedHandle);
		_itemFromInventoryDiscardedHandle.Reset();
	}

	_cleanupQuantityBeingSet();
}

TWeakInterfacePtr<IItemInventoryWidget> UInventoryPresenterWidget::GetInventoryWidget() {
	return _inventoryWidget;
}

void UInventoryPresenterWidget::_onClose() {
	OnCloseFromUI().Broadcast();
}

void UInventoryPresenterWidget::_onItemFromInventoryBeingDiscarded(TWeakInterfacePtr<IItem> item, TWeakInterfacePtr<IInventory> inventory, const FPointerEvent& mouseEvent) {
	if (!mouseEvent.IsShiftDown() || !item->IsStackable()) {
		OnItemFromInventoryBeingDropped().Broadcast(MoveTemp(item), item->GetQuantity(), MoveTemp(inventory));
		return;
	}

	// Spawn the UQuantitySetterWidget if this is the first time we drop an item
	if (!IsValid(_quantitySetter)) {
		check(IsValid(_quantitySetterClass));
		_quantitySetter = CreateWidget<UQuantitySetterWidget>(this, _quantitySetterClass);
	}

	// Make the UQuantitySetterWidget appear where the user clicked
	const auto quantitySetterSlot = _mainOverlay->AddChildToOverlay(_quantitySetter);
	const auto worldToOverlay = _mainOverlay->GetCachedGeometry().GetAccumulatedRenderTransform().Inverse();
	const auto mouseInOverlayRF = worldToOverlay.TransformPoint(mouseEvent.GetScreenSpacePosition());
	quantitySetterSlot->SetPadding(FMargin{static_cast<float>(mouseInOverlayRF.X), static_cast<float>(mouseInOverlayRF.Y)});

	// Bind the quantity text box
	_quantitySetter->GetQuantityTextBox()->OnTextCommitted.AddDynamic(this, &UInventoryPresenterWidget::_onQuantitySetterCommit);
	_quantitySetter->GetQuantityTextBox()->SetKeyboardFocus();
	_itemDropPayload = {MoveTemp(item), MoveTemp(inventory)};
}

void UInventoryPresenterWidget::_onQuantitySetterCommit(const FText& quantityText, const ETextCommit::Type commitType) {
	if (commitType != ETextCommit::OnEnter) {
		// Fun fact: this is called if SetKeyboardFocus() on the _quantitySetter. This is why we don't RemoveDynamic here;
		// the setter would disappear immediately
		return;
	}
	
	int32 quantity = FCString::Atoi(*quantityText.ToString());
	if (quantity == 0) {
		// This could be either because Atoi had some problem converting the given text or because the player actually
		// wrote 0. Either way, we should do nothing.
		_cleanupQuantityBeingSet();
		return;
	}

	check(_itemDropPayload.IsValid());

	quantity = FMath::Min(quantity, _itemDropPayload.ItemPendingDrop->GetQuantity());
	OnItemFromInventoryBeingDropped().Broadcast(MoveTemp(_itemDropPayload.ItemPendingDrop), quantity, MoveTemp(_itemDropPayload.InventoryPendingDrop));
	_cleanupQuantityBeingSet();
}

void UInventoryPresenterWidget::_cleanupQuantityBeingSet() {
	if (IsValid(_quantitySetter)) {
		_quantitySetter->GetQuantityTextBox()->OnTextCommitted.RemoveDynamic(this, &UInventoryPresenterWidget::_onQuantitySetterCommit);
		_quantitySetter->RemoveFromParent();	
	}
	_itemDropPayload.Reset();
}
