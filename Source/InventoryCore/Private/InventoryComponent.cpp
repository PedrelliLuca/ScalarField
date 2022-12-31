// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Algo/Accumulate.h"
#include "Algo/AnyOf.h"

#define LOCTEXT_NAMESPACE "InventoryComponent"

UInventoryComponent::UInventoryComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

double UInventoryComponent::GetCurrentWeight() const {
	return Algo::Accumulate(_items, 0.0, [](double accumulation, const TObjectPtr<UInventoryItem>& item) {
		return accumulation + item->GetStackWeight();
	});
}

double UInventoryComponent::GetCurrentVolume() const {
	return Algo::Accumulate(_items, 0.0, [](double accumulation, const TObjectPtr<UInventoryItem>& item) {
		return accumulation + item->GetStackVolume();
	});
}

TArray<TWeakInterfacePtr<IItem>> UInventoryComponent::GetItems() const {
	TArray<TWeakInterfacePtr<IItem>> abstractItems;
	abstractItems.Reserve(_items.Num());
	
	for (const auto& item : _items) {
		TWeakInterfacePtr<IItem> abstractItem = Cast<IItem>(item);
		check(abstractItem.IsValid());
		abstractItems.Emplace(MoveTemp(abstractItem));
	}

	return abstractItems;
}

FItemAddResult UInventoryComponent::TryAddItem(TWeakInterfacePtr<IItem> item) {
	check(item.IsValid());

	const int32 amountToAdd = item->GetQuantity();

	// Can we add at least one item? Let's check the weight and volume to find out
	const double itemWeight = item->GetWeight();
	if (!FMath::IsNearlyZero(itemWeight) && GetCurrentWeight() + itemWeight > _weightCapacity) {
		return FItemAddResult::AddedNone(amountToAdd, LOCTEXT("InventoryTooMuchWeightText", "Couldn't add item to inventory, carrying too much weight."));
	}

	const double itemVolume = item->GetVolume();
	if (!FMath::IsNearlyZero(itemVolume) && GetCurrentVolume() + itemVolume > _volumeCapacity) {
		return FItemAddResult::AddedNone(amountToAdd, LOCTEXT("InventoryTooLittleVolumeText", "Not enough space to carry item."));
	}

	/* NOTE: given an item BP class, we allow only a single stack for that class. For example, you can't have 2 stacks of health potions. However, you can
	have a stack of health potions and a stack of mana potions, resulting in 2 stacks of potions. */

	if (item->IsStackable()) {
		/* Do we already have a stack of this item in our inventory ? In this case we may need to modify the quantity to add
		based on how much we already have on the existing stack */
		const auto existingItem = FindItemByClass(item.GetObject()->GetClass());
		if (existingItem.IsValid() && existingItem->GetMaxQuantity() == existingItem->GetQuantity()) {
			return FItemAddResult::AddedNone(amountToAdd, FText::Format(LOCTEXT("ItemStackOverflowText", "The existing stack of {ItemName} is full, can't add more {ItemName}."), item->GetNameText()));
		}

		int32 amountAddable = existingItem.IsValid() ? FMath::Min(amountToAdd, existingItem->GetMaxQuantity() - existingItem->GetQuantity()) : amountToAdd;
		FText errorText{};
		if (amountAddable < amountToAdd) {
			errorText = FText::Format(LOCTEXT("ItemStackOverflowText", "The existing stack of {ItemName} in your inventory is too big to add all of the item."), item->GetNameText());
		}

		if (!FMath::IsNearlyZero(itemWeight) || !FMath::IsNearlyZero(itemVolume)) {
			const int32 amountAddableBasedOnWeight = !FMath::IsNearlyZero(itemWeight) ? FMath::FloorToInt32((_weightCapacity - GetCurrentWeight()) / item->GetWeight()) : amountAddable;
			amountAddable = FMath::Min(amountAddable, amountAddableBasedOnWeight);

			// Does the weight restrict the amount of item we can take?
			if (amountAddable == amountAddableBasedOnWeight) {
				errorText = FText::Format(LOCTEXT("InventoryTooMuchWeightText", "You are carrying too much weight to add the entire stack of {ItemName} to the inventory."), item->GetNameText());
			}

			const int32 amountAddableBasedOnVolume = !FMath::IsNearlyZero(itemVolume) ? FMath::FloorToInt32((_volumeCapacity - GetCurrentVolume()) / item->GetVolume()) : amountAddable;
			amountAddable = FMath::Min(amountAddable, amountAddableBasedOnVolume);

			// Does the volume restrict the amount of item we can take?
			if (amountAddable == amountAddableBasedOnVolume) {
				errorText = FText::Format(LOCTEXT("InventoryTooLittleVolumeText", "You have too little space left to add the entire stack of {ItemName} to the inventory."), item->GetNameText());
			}
		}
			
		/* If one of the checks above reduced amountAddable to zero, it means that we can't add any items to our inventory. And since we do not allow multiple stacks of the same
		kind, we return an AddedNone() result. */
		if (amountAddable == 0) {
			return FItemAddResult::AddedNone(amountToAdd, FText::Format(LOCTEXT("InventoryErrorText", "Couldn't add any of the {ItemName} to the ivnentory."), item->GetNameText()));
		}

		if (existingItem.IsValid()) {
			existingItem->SetQuantity(existingItem->GetQuantity() + amountAddable);
			check(existingItem->GetQuantity() <= existingItem->GetMaxQuantity());
			if (amountAddable < amountToAdd) {
				return FItemAddResult::AddedSome(amountToAdd, amountAddable, errorText);
			}
			
			return FItemAddResult::AddedAll(amountToAdd);
		}

		TObjectPtr<UInventoryItem> addableItem = NewObject<UInventoryItem>(this, item.GetObject()->GetClass());
		addableItem->SetQuantity(amountAddable);
		_addItem(addableItem);

		if (amountAddable < amountToAdd) {
			return FItemAddResult::AddedSome(amountToAdd, amountAddable, errorText);
		}
			
		return FItemAddResult::AddedAll(amountToAdd);
	}
	
	// The item is not stackable or stackable but we don't have an existing stack of it in the inventory. In any case, we simply add all of it to the inventory
	_addItem(item);
	return FItemAddResult::AddedAll(item->GetQuantity());
}

FItemAddResult UInventoryComponent::TryAddItemFromClass(const TSubclassOf<UInventoryItem> itemClass, const int32 quantity) {
	TObjectPtr<UInventoryItem> item = NewObject<UInventoryItem>(this, itemClass);
	item->SetQuantity(quantity);
	return TryAddItem(item);
}

int32 UInventoryComponent::ConsumeAllOfItem(const TObjectPtr<UInventoryItem> item) {
	check(IsValid(item));
	return ConsumeItem(item, item->GetQuantity());
}

bool UInventoryComponent::RemoveItem(TWeakInterfacePtr<IItem> item) {
	const auto concreteItem = Cast<UInventoryItem>(item.GetObject());
	check(IsValid(concreteItem));
	
	const int32 nRemovedItems = _items.Remove(concreteItem);
	check(nRemovedItems < 2);

	if (nRemovedItems == 0) {
		return false;
	}

	OnInventoryUpdated().Broadcast();
	return true;
}

int32 UInventoryComponent::ConsumeItem(TWeakInterfacePtr<IItem> item, const int32 quantity) {
	check(item.IsValid());
	const int32 quantityActuallyConsumed = FMath::Min(quantity, item->GetQuantity());

	item->SetQuantity(item->GetQuantity() - quantityActuallyConsumed);
	if (item->GetQuantity() == 0) {
		RemoveItem(item);
	} else {
		OnInventoryUpdated().Broadcast();
	}

	return quantityActuallyConsumed;
}

bool UInventoryComponent::HasItemOfClass(const TSubclassOf<UInventoryItem> itemClass, const int32 quantity) const {
	return Algo::AnyOf(_items, [&itemClass, &quantity](const TObjectPtr<UInventoryItem>& item) {
		return item->IsA(itemClass) && item->GetQuantity() >= quantity;
	});
}

TWeakInterfacePtr<IItem> UInventoryComponent::FindItemByClass(TSubclassOf<UObject> itemClass) {
	if (!itemClass->ImplementsInterface(UItem::StaticClass())) {
		return nullptr;
	}

	const auto itemPtr = Algo::FindByPredicate(_items, [&itemClass](const TObjectPtr<UInventoryItem>& item) {
		check(IsValid(item));
		return item->IsA(itemClass);
	});

	if (itemPtr == nullptr) {
		return nullptr;
	}

	const TWeakInterfacePtr<IItem> item = *itemPtr;
	check(item.IsValid());
	return item;
}

TArray<TWeakObjectPtr<UInventoryItem>> UInventoryComponent::FindItemsByClass(const TSubclassOf<UInventoryItem> itemClass) {
	const auto itemsOfClass = _items.FilterByPredicate([&itemClass](const TObjectPtr<UInventoryItem>& item) {
		check(IsValid(item));
		return item->IsA(itemClass);
	});

	TArray<TWeakObjectPtr<UInventoryItem>> outItems;
	outItems.Reserve(itemsOfClass.Num());
	Algo::Transform(itemsOfClass, outItems, [](const TObjectPtr<UInventoryItem>& item) {
		return TWeakObjectPtr<UInventoryItem>(item);
	});

	return outItems;
}

TObjectPtr<UInventoryItem> UInventoryComponent::_addItem(const TWeakInterfacePtr<IItem> item) {
    TObjectPtr<UInventoryItem> newItem = Cast<UInventoryItem>(item.GetObject());
	if (newItem->GetOuter() != this) {
		// The item we're trying to add is not owned by this inventory component => we make a copy of it with us as outer.
		newItem = NewObject<UInventoryItem>(this, item.GetObject()->GetClass());
		newItem->SetQuantity(item->GetQuantity());
	}

	newItem->OnItemAddedToInventory(this);
	_items.Emplace(newItem);
	OnInventoryUpdated().Broadcast();
	return newItem;
}

#undef LOCTEXT_NAMESPACE