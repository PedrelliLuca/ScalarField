// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "InventoryItem.h"

#include "InventoryItemWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemDoubleClick, TWeakObjectPtr<UInventoryItem>);

UCLASS()
class INVENTORYCORE_API UInventoryItemWidget : public UUserWidget {
     GENERATED_BODY()
  
public:
    void SetItem(const TObjectPtr<UInventoryItem>& inventoryItem);

	FOnItemDoubleClick& OnItemDoubleClick() { return _onDoubleClick; }

	UFUNCTION(BlueprintImplementableEvent)
	void RefreshWidget();

protected:
	FReply NativeOnMouseButtonDoubleClick(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) override;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory Item Widget", meta = (ExposeOnSpawn = true))
    TObjectPtr<UInventoryItem> _inventoryItem = nullptr;

private:
	FOnItemDoubleClick _onDoubleClick;
};
