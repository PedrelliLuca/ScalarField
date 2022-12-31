// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryContainerWidgetInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventoryToggleController.h"
#include "ItemUsageController.h"
#include "PickupSpawnController.h"

#include "InventoryManipulationSubsystem.generated.h"

/*!
 * \brief Subsystem to perform operations on inventories
 */
UCLASS()
class INVENTORYMANIPULATION_API UInventoryManipulationSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& collection) override;

	void SetInventoryContainerWidget(TWeakInterfacePtr<IInventoryContainerWidget> inventoryContainer);
	void SetHUDToShowOnClose(TWeakInterfacePtr<IPawnBindableWidget> widgetOnClose);
     
	void OpenInventoryOfActor(TWeakObjectPtr<AActor> actor);
	void CloseInventory();

	FOnInventoryClosedFromUI& OnInventoryClosedFromUI() { return _inventoryToggleController->OnInventoryClosedFromUI(); }

private:
	UPROPERTY()
	TObjectPtr<UInventoryToggleController> _inventoryToggleController = nullptr;

	UPROPERTY()
	TObjectPtr<UItemUsageController> _itemUsageController = nullptr;

	UPROPERTY()
	TObjectPtr<UPickupSpawnController> _pickupSpawnController = nullptr;
};
