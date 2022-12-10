// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetsPresenterComponent.h"

void UWidgetsPresenterComponent::SetOwnerPlayerController(TWeakObjectPtr<APlayerController> playerController) {
	check(playerController.IsValid());
	_ownerPlayerController = playerController;
	_createHUD();
}

void UWidgetsPresenterComponent::_createHUD() {
	if (!IsValid(_hudWidgetClass)) {
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UHUDWidget class. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	if (!IsValid(_hudWidget)) {
		_hudWidget = CreateWidget<UHUDWidget>(_ownerPlayerController.Get(), _hudWidgetClass);
	}
	_hudWidget->AddToViewport();
	_hudWidget->BindToPawn(_ownerPlayerController->GetPawn());
}


