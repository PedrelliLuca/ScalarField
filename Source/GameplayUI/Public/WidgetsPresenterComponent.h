// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDWidget.h"
#include "Components/ActorComponent.h"

#include "WidgetsPresenterComponent.generated.h"

UCLASS()
class GAMEPLAYUI_API UWidgetsPresenterComponent : public UActorComponent {
     GENERATED_BODY()
     
public:
     void SetOwnerPlayerController(TWeakObjectPtr<APlayerController> playerController);

private:
     void _createHUD();
     
     TWeakObjectPtr<APlayerController> _ownerPlayerController;

     UPROPERTY(EditAnywhere, Category = "Widgets")
     TSubclassOf<UHUDWidget> _hudWidgetClass = nullptr;

     UPROPERTY()
     TObjectPtr<UHUDWidget> _hudWidget = nullptr;
};
