// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetsPresenterComponent.h"

void UWidgetsPresenterComponent::SetOwnerPlayerController(TWeakObjectPtr<APlayerController> playerController) {
	check(playerController.IsValid());
	_ownerPlayerController = playerController;
	_createHUD();
	_createInventoryPresenter();
}

bool UWidgetsPresenterComponent::IsInventoryOnViewport() const {
	return _inventoryPresenterWidget->IsInViewport();
}

void UWidgetsPresenterComponent::ShowInventory(TWeakObjectPtr<UInventoryComponent> inventoryComponent) {
	check(inventoryComponent.IsValid());
	_hudWidget->RemoveFromParent();
	_hudWidget->RemovePawnBindings();
	_inventoryPresenterWidget->AddToViewport();
}

void UWidgetsPresenterComponent::HideInventory() {
	_inventoryPresenterWidget->RemoveFromParent();
	_hudWidget->AddToViewport();
	_hudWidget->BindToPawn(_ownerPlayerController->GetPawn());
}

void UWidgetsPresenterComponent::_createHUD() {
	if (!IsValid(_hudWidgetClass)) {
		UE_LOG(LogTemp, Error, TEXT("%s() Missing hud class. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	if (!IsValid(_hudWidget)) {
		_hudWidget = CreateWidget<UHUDWidget>(_ownerPlayerController.Get(), _hudWidgetClass);
	}
	_hudWidget->AddToViewport();
	_hudWidget->BindToPawn(_ownerPlayerController->GetPawn());
}

void UWidgetsPresenterComponent::_createInventoryPresenter() {
	if (!IsValid(_inventoryPresenterWidgetClass)) {
		UE_LOG(LogTemp, Error, TEXT("%s() Missing inventory presenter class. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	if (!IsValid(_inventoryPresenterWidget)) {
		_inventoryPresenterWidget = CreateWidget<UInventoryPresenterWidget>(_ownerPlayerController.Get(), _inventoryPresenterWidgetClass);
	}
}


