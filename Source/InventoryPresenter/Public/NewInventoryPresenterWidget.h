// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContainerWidgetInterface.h"
#include "NewInventoryWidget.h"
#include "Blueprint/UserWidget.h"

#include "NewInventoryPresenterWidget.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnButtonClose);

UCLASS()
class INVENTORYPRESENTER_API UNewInventoryPresenterWidget : public UUserWidget, public IInventoryContainerWidget {
     GENERATED_BODY()
  
public:
    void ShowInventory(TWeakInterfacePtr<IInventory> inventory) override;
	void HideInventory() override;
	TWeakInterfacePtr<IItemInventoryWidget> GetInventoryWidget() override;

private:
	UFUNCTION(BlueprintCallable)
	void _onClose();
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNewInventoryWidget> _inventoryWidget;

};
