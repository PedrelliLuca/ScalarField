// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDWidget.h"
#include "InventoryComponent.h"
#include "Components/ActorComponent.h"

#include "WidgetsPresenterComponent.generated.h"

/**
 * \brief Component that manages how the widgets behave with respect to each other
 */
UCLASS()
class GAMEPLAYUI_API UWidgetsPresenterComponent : public UActorComponent {
     GENERATED_BODY()
     
public:
     void SetOwnerPlayerController(TWeakObjectPtr<APlayerController> playerController);
     bool IsInventoryOnViewport() const;
     void ShowInventory(TWeakObjectPtr<UInventoryComponent> inventoryComponent);
     void HideInventory();

private:
     void _createHUD();
     void _createInventoryPresenter();
     
     TWeakObjectPtr<APlayerController> _ownerPlayerController;

     UPROPERTY(EditDefaultsOnly, Category = "Widgets")
     TSubclassOf<UHUDWidget> _hudWidgetClass = nullptr;

     UPROPERTY()
     TObjectPtr<UHUDWidget> _hudWidget = nullptr;

     UPROPERTY(EditDefaultsOnly, Category = "Widgets")
     TSubclassOf<UUserWidget> _inventoryPresenterWidgetClass = nullptr;

     UPROPERTY()
     TObjectPtr<UUserWidget> _inventoryPresenterWidget = nullptr;
};
