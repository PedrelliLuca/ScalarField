// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPresenterWidget.h"

#include "Components/OverlaySlot.h"
#include "Components/WidgetComponent.h"

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

FReply UInventoryPresenterWidget::NativeOnMouseButtonDown(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) {
	/* We don't want to perform the box check if one of the following is true:
	 * 1. The setter was drawn this frame, because the paint space geometry would be invalid in such case, causing the
	 * "is inside" check below to cleanup the setter immediately.
	 * 2. The setter has never been initialized before. This can easily happen: _quantitySetter is initialized on the
	 * first shift-right click, but the user might decide to never do that and just click normally.
	 * 3. The setter is initialized but is not a child of the main overlay. This means that it is valid but not on screen.
	 */
	if (_bQuantitySetterJustDrawn || !IsValid(_quantitySetter) || !_mainOverlay->HasChild(_quantitySetter)) {
		return Super::NativeOnMouseButtonDown(inGeometry, inMouseEvent);
	}

	// Box check. Did we click outside the quantitySetter? If so, we make it disappear, aborting the setting of the quantity
	const auto location = _quantitySetter->GetPaintSpaceGeometry().GetAbsolutePosition();
	const auto size = _quantitySetter->GetPaintSpaceGeometry().GetAbsoluteSize();
	const FBox2d quantitySetterWidgetBox{location, location + size};
	
	if (!quantitySetterWidgetBox.IsInside(inMouseEvent.GetScreenSpacePosition())) {
		_cleanupQuantityBeingSet();
	}
	
	return Super::NativeOnMouseButtonDown(inGeometry, inMouseEvent);
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

	// If we already have the quantity setter widget out and we shift-right click somewhere else we need to perform a
	// cleanup
	if (_mainOverlay->HasChild(_quantitySetter)) {
		_cleanupQuantityBeingSet();
	}

	// Make the UQuantitySetterWidget appear where the user clicked
	const auto quantitySetterSlot = _mainOverlay->AddChildToOverlay(_quantitySetter);
	const auto worldToOverlay = _mainOverlay->GetCachedGeometry().GetAccumulatedRenderTransform().Inverse();
	const auto mouseInOverlayRF = worldToOverlay.TransformPoint(mouseEvent.GetScreenSpacePosition());
	quantitySetterSlot->SetPadding(FMargin{static_cast<float>(mouseInOverlayRF.X), static_cast<float>(mouseInOverlayRF.Y)});

	// Tell NativeOnMouseButtonDown() that this frame we won't perform the box check
	_bQuantitySetterJustDrawn = true;
	
	GetWorld()->GetTimerManager().SetTimerForNextTick([&justDrawn = _bQuantitySetterJustDrawn, &quantitySetter = _quantitySetter](){
		justDrawn = false;
		check(IsValid(quantitySetter));

		// We need to wait a frame for SetKeyboardFocus to work, it employs the cached widget.
		quantitySetter->GetQuantityTextBox()->SetKeyboardFocus();
	});
	
	// Bind the quantity text box
	_quantitySetter->GetQuantityTextBox()->OnTextCommitted.AddDynamic(this, &UInventoryPresenterWidget::_onQuantitySetterCommit);
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
