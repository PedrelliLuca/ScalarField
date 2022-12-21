// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

#define LOCTEXT_NAMESPACE "Item"

UInventoryItem::UInventoryItem() {
	_nameText = LOCTEXT("Name", "Item");
	_actionText = LOCTEXT("Action", "Use");
	_weight = 0.0;
	_volume = 0.0;
	_bIsStackable = false;
	_quantity = 1;
	_maxQuantity = 2;  
}

#if WITH_EDITOR
void UInventoryItem::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
	Super::PostEditChangeProperty(propertyChangedEvent);
	const auto property = propertyChangedEvent.Property;
	const auto propertyName = property != nullptr ? property->GetFName() : NAME_None;
	if (propertyName == GET_MEMBER_NAME_CHECKED(UInventoryItem, _quantity)) {
		_quantity = FMath::Clamp(_quantity, 0, _bIsStackable ? _maxQuantity : 1);
	} else if (propertyName == GET_MEMBER_NAME_CHECKED(UInventoryItem, _bIsStackable) && !_bIsStackable) {
		_quantity = 1;
	}
}
#endif

void UInventoryItem::SetQuantity(const int32 newQuantity) {
	if (_quantity == newQuantity) {
		return;
	}

	_quantity = FMath::Clamp(newQuantity, 0, _bIsStackable ? _maxQuantity : 1);
	_onItemModified.Broadcast();
}

#undef LOCTEXT_NAMESPACE
