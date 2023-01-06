// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "InventoryContainerWidgetInterface.h"
#include "InventoryWidget.h"
#include "QuantitySetterWidget.h"

#include "InventoryPresenterWidget.generated.h"

UCLASS()
class INVENTORYPRESENTER_API UInventoryPresenterWidget : public UUserWidget, public IInventoryContainerWidget {
     GENERATED_BODY()
  
public:
    void ShowInventory(TWeakInterfacePtr<IInventory> inventory) override;
	void HideInventory() override;

protected:
	FReply NativeOnMouseButtonDown(const FGeometry& inGeometry, const FPointerEvent& inMouseEvent) override;
	
private:
	UFUNCTION(BlueprintCallable)
	void _onClose();

	void _onItemFromInventoryBeingUsed(TWeakInterfacePtr<IItem> item, TWeakInterfacePtr<IInventory> inventory);
	
	void _onItemFromInventoryBeingDiscarded(TWeakInterfacePtr<IItem> item, TWeakInterfacePtr<IInventory> inventory, const FPointerEvent& mouseEvent);

	UFUNCTION()
	void _onQuantitySetterCommit(const FText& quantityText, ETextCommit::Type commitType);

	void _cleanupQuantityBeingSet();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryWidget> _inventoryWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> _mainOverlay;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UQuantitySetterWidget> _quantitySetterClass = nullptr;
     
	UPROPERTY()
	TObjectPtr<UQuantitySetterWidget> _quantitySetter = nullptr;

	FDelegateHandle _itemFromInventoryUsedHandle{};
	FDelegateHandle _itemFromInventoryDiscardedHandle{};

	struct FItemDropPayload {
		bool IsValid() const { return ItemPendingDrop.IsValid() && InventoryPendingDrop.IsValid(); }
		void Reset() {
			ItemPendingDrop = nullptr;
			InventoryPendingDrop = nullptr;
		}
		
		TWeakInterfacePtr<IItem> ItemPendingDrop = nullptr;
		TWeakInterfacePtr<IInventory> InventoryPendingDrop = nullptr;
	};

	FItemDropPayload _itemDropPayload{};

	bool _bQuantitySetterJustDrawn = false;
};
