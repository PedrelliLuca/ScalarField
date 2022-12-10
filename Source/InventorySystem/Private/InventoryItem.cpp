// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"

#define LOCTEXT_NAMESPACE "Item"

UInventoryItem::UInventoryItem() {
	_name = LOCTEXT("Name", "Item");
	_action = LOCTEXT("Action", "Use");
	_weight = 0.0;
	_bIsStackable = false;
	_quantity = 1;
	_maxQuantity = 2;  
}

bool UInventoryItem::ShouldShowInInventory() const {
	return true;
}

#undef LOCTEXT_NAMESPACE
