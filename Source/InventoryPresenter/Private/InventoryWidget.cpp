// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"

#include "Components/OverlaySlot.h"

void UInventoryWidget::SetInventory(TWeakInterfacePtr<IInventory> inventory) {
	if (_inventory.IsValid()) {
		_inventory->OnInventoryUpdated().RemoveAll(this);
	}

	check(inventory.IsValid());
	_inventory = MoveTemp(inventory);
	_inventory->OnInventoryUpdated().AddUObject(this, &UInventoryWidget::_refreshInventoryItems);

	_currentClassFilter = nullptr;
	_refreshInventoryItems();
}

void UInventoryWidget::_filterItemsByClass(TSubclassOf<UObject> itemClass /*= nullptr*/) {
	check(IsValid(_itemWidgetClass));
	_inventoryItemsBox->ClearChildren();

#if DO_CHECK
	if (IsValid(itemClass)) {
		check(itemClass->ImplementsInterface(UItem::StaticClass()));
	}
#endif
	
	for (const auto& item : _inventory->GetItems()) {
		if (IsValid(itemClass) && !item.GetObject()->IsA(itemClass)) {
			continue;
		}
		
		const auto itemWidget = CreateWidget<UInventoryItemWidget>(GetOwningPlayer(), _itemWidgetClass);
		itemWidget->SetItem(item);
		itemWidget->OnItemUsage().AddUObject(this, &UInventoryWidget::_onItemBeingUsed);
		itemWidget->OnItemDiscarded().AddUObject(this, &UInventoryWidget::_onItemBeingDropped);

		_inventoryItemsBox->AddChildToWrapBox(itemWidget);
	}

	_currentClassFilter = MoveTemp(itemClass);

	_refreshInventoryInformation();
}


void UInventoryWidget::_refreshInventoryItems() {
	_filterItemsByClass(_currentClassFilter);
}

void UInventoryWidget::_onItemBeingUsed(TWeakInterfacePtr<IItem> item) {
	// In the future, the quantity to use will be set using an additional widget. For the moment, we'll just use 1 of
	// the item.
	OnItemFromInventoryBeingUsed().Broadcast(item, 1, _inventory);
}

void UInventoryWidget::_onItemBeingDropped(TWeakInterfacePtr<IItem> item, const FPointerEvent& mouseEvent) {
	if (!mouseEvent.IsShiftDown() || !item->IsStackable()) {
		OnItemFromInventoryBeingDropped().Broadcast(MoveTemp(item), item->GetQuantity(), _inventory);
		return;
	}

	// Spawn the UQuantitySetterWidget if this is the first time we drop an item
	if (!IsValid(_quantitySetter)) {
		check(IsValid(_quantitySetterClass));
		_quantitySetter = CreateWidget<UQuantitySetterWidget>(this, _quantitySetterClass);
	}

	// Make the UQuantitySetterWidget appear where the user clicked
	const auto quantitySetterSlot = _inventoryOverlay->AddChildToOverlay(_quantitySetter);
	const auto worldToOverlay = _inventoryOverlay->GetCachedGeometry().GetAccumulatedRenderTransform().Inverse();
	const auto mouseInOverlayRF = worldToOverlay.TransformPoint(mouseEvent.GetScreenSpacePosition());
	quantitySetterSlot->SetPadding(FMargin{static_cast<float>(mouseInOverlayRF.X), static_cast<float>(mouseInOverlayRF.Y)});

	// Bind the quantity text box
	_quantitySetter->GetQuantityTextBox()->OnTextCommitted.AddDynamic(this, &UInventoryWidget::_sanitizeItemQuantity);
	_quantitySetter->GetQuantityTextBox()->SetKeyboardFocus();
	_itemPendingDrop = MoveTemp(item);

	/* 1. Create widget
	 * 2. add it to viewport at click location https://forums.unrealengine.com/t/setting-a-widgets-position-to-mouse-position/423645 https://forums.unrealengine.com/t/set-position-in-viewport-widget-not-working/692335/2
	 * 3. set focus on it
	 * 4. On text commit send here the quantity (you'll need to make a version of this function that takes the quantity or something) and destroy it
	 * 5. If focus is lost, well, just destroy. https://forums.unrealengine.com/t/how-to-remove-ui-widget-when-focus-lost/357575
	 */
	
	// _quantitySetter->OnQuantityCommitted().AddLambda([this, item](const int32 quantity) {
	// OnItemFromInventoryBeingDropped().Broadcast(item, FMath::Min(quantity, item->GetQuantity()), _inventory);
	// });
}

void UInventoryWidget::_sanitizeItemQuantity(const FText& quantityText, ETextCommit::Type commitType) {
	if (commitType != ETextCommit::OnEnter) {
		// Fun fact: this is called if SetKeyboardFocus() on the _quantitySetter. This is why we don't RemoveDynamic here;
		// the setter would disappear immediately
		return;
	}
	
	int32 quantity = FCString::Atoi(*quantityText.ToString());
	if (quantity == 0) {
		// This could be either because Atoi had some problem converting the given text or because the player actually
		// wrote 0. Either way, we should do nothing.
		_quantitySetter->RemoveFromParent();
		_quantitySetter->GetQuantityTextBox()->OnTextCommitted.RemoveDynamic(this, &UInventoryWidget::_sanitizeItemQuantity);
		_itemPendingDrop = nullptr;
		return;
	}

	check(_itemPendingDrop.IsValid());

	quantity = FMath::Min(quantity, _itemPendingDrop->GetQuantity());
	OnItemFromInventoryBeingDropped().Broadcast(MoveTemp(_itemPendingDrop), quantity, _inventory);
	_quantitySetter->RemoveFromParent();
	_quantitySetter->GetQuantityTextBox()->OnTextCommitted.RemoveDynamic(this, &UInventoryWidget::_sanitizeItemQuantity);
}
