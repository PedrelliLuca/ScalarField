// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "WidgetsPresenterComponent.generated.h"

class IInventoryContainerWidget;
class IPawnBindableWidget;
class UHUDWidget;
class UInventoryPresenterWidget;
class USkillsContainerComponent;
class USkillIconContainerWidget;

/**
 * \brief Component that stores the widgets associated with the onwer player controlelr
 */
UCLASS()
class WIDGETSPRESENTATION_API UWidgetsPresenterComponent : public UActorComponent {
    GENERATED_BODY()

public:
    TWeakInterfacePtr<IPawnBindableWidget> GetHUDWidget();
    TWeakInterfacePtr<IInventoryContainerWidget> GetInventoryContainerWidget();

    void CreateAllWidgets();
    void ProvideSkillsToWidgets(TObjectPtr<USkillsContainerComponent> skillsContainer);

private:
    void _createHUD(const TWeakObjectPtr<APlayerController>& ownerPlayerC);
    void _createInventoryPresenter(const TWeakObjectPtr<APlayerController>& ownerPlayerC);

    UPROPERTY(EditDefaultsOnly, Category = "Pickup")
    TSubclassOf<UHUDWidget> _hudWidgetClass = nullptr;

    UPROPERTY()
    TObjectPtr<UHUDWidget> _hudWidget = nullptr;

    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UInventoryPresenterWidget> _inventoryPresenterWidgetClass = nullptr;

    UPROPERTY()
    TObjectPtr<UInventoryPresenterWidget> _inventoryPresenterWidget = nullptr;

    UPROPERTY()
    TObjectPtr<USkillIconContainerWidget> _skillIconContainerWidget = nullptr;
};
