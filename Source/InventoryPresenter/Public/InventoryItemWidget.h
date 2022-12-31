// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "ItemInterface.h"
#include "ItemWidgetInterface.h"

#include "InventoryItemWidget.generated.h"

UCLASS()
class INVENTORYPRESENTER_API UInventoryItemWidget : public UUserWidget, public IItemWidget {
     GENERATED_BODY()
  
public:
	void SetItem(TWeakInterfacePtr<IItem> item) override;

	UFUNCTION(BlueprintImplementableEvent)
	void RefreshWidget();

	UFUNCTION(BlueprintPure)
	TScriptInterface<IItem> GetItem() const { return _item.GetObject(); }
	
protected:
	FReply NativeOnMouseButtonDoubleClick(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) override;
	FReply NativeOnMouseButtonDown(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) override;

	TWeakInterfacePtr<IItem> _item = nullptr;
};
