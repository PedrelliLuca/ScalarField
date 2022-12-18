// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySubsystem.h"

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& collection) {
	Super::Initialize(collection);

	_inventoryController = NewObject<UInventoryController>(this, UInventoryController::StaticClass());
}

void UInventorySubsystem::SetWidgetsPresenter(TWeakObjectPtr<UWidgetsPresenterComponent> widgetsPresenterC) {
	_inventoryController->SetWidgetsPresenter(MoveTemp(widgetsPresenterC));
}

void UInventorySubsystem::ShowInventoryOfActor(TWeakObjectPtr<AActor> actor) {
	_inventoryController->ShowInventoryOfActor(MoveTemp(actor));
}
