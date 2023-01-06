// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "ItemInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "InventoryItemWidget.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUsage, TWeakInterfacePtr<IItem>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDiscarded, TWeakInterfacePtr<IItem>, const FPointerEvent&);

UCLASS()
class INVENTORYPRESENTER_API UInventoryItemWidget : public UUserWidget {
     GENERATED_BODY()
  
public:
	void SetItem(TWeakInterfacePtr<IItem> item);

	UFUNCTION(BlueprintImplementableEvent)
	void RefreshWidget();

	UFUNCTION(BlueprintPure)
	TScriptInterface<IItem> GetItem() const { return _item.GetObject(); }

	FOnItemUsage& OnItemUsage() { return _onItemUsage; }
	FOnItemDiscarded& OnItemDiscarded() { return _onItemDiscarded; }
	
protected:
	FReply NativeOnMouseButtonDoubleClick(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) override;
	FReply NativeOnMouseButtonDown(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) override;

	TWeakInterfacePtr<IItem> _item = nullptr;

private:
	FOnItemUsage _onItemUsage{};
	FOnItemDiscarded _onItemDiscarded{};
};
