// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryController.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WidgetsPresenterComponent.h"

#include "InventorySubsystem.generated.h"

/*!
 * \brief System that allows to show inventories on screen
 */
UCLASS()
class INVENTORYSERVICE_API UInventorySubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& collection) override;

	void SetWidgetsPresenter(TWeakObjectPtr<UWidgetsPresenterComponent> widgetsPresenterC);

	void ShowInventoryOfActor(TWeakObjectPtr<AActor> actor);

	FOnInventoryWidgetClosed& OnInventoryWidgetClosed() { return _inventoryController->OnInventoryWidgetClosed(); }

private:
	UPROPERTY()
	TObjectPtr<UInventoryController> _inventoryController = nullptr;
};