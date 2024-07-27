// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetsPresenterComponent.h"

TWeakInterfacePtr<IPawnBindableWidget> UWidgetsPresenterComponent::GetHUDWidget() {
    TWeakInterfacePtr<IPawnBindableWidget> asPawnBindable = Cast<IPawnBindableWidget>(_hudWidget);
    check(asPawnBindable.IsValid());
    return asPawnBindable;
}

TWeakInterfacePtr<IInventoryContainerWidget> UWidgetsPresenterComponent::GetInventoryContainerWidget() {
    TWeakInterfacePtr<IInventoryContainerWidget> asInventoryContainer = Cast<IInventoryContainerWidget>(_inventoryPresenterWidget);
    check(asInventoryContainer.IsValid());
    return asInventoryContainer;
}

void UWidgetsPresenterComponent::CreateAllWidgets() {
    const TWeakObjectPtr<APlayerController> playerC = Cast<APlayerController>(GetOwner());
    check(playerC.IsValid());

    _createHUD(playerC);
    _createInventoryPresenter(playerC);
}

void UWidgetsPresenterComponent::ProvideSkillsToWidgets(TObjectPtr<USkillsContainerComponent> skillsContainer) {
    // TODO: Send the skills to the HUD's SkillIconContainerWidget, which then will take care of creating the SkillIconWidgets and setting them up.
}

void UWidgetsPresenterComponent::_createHUD(const TWeakObjectPtr<APlayerController>& ownerPlayerC) {
    check(ownerPlayerC.IsValid());
    check(IsValid(_hudWidgetClass));

    _hudWidget = CreateWidget<UHUDWidget>(ownerPlayerC.Get(), _hudWidgetClass);
    _hudWidget->SetPawn(ownerPlayerC->GetPawn());
    _hudWidget->BindCurrentPawn();
    _hudWidget->AddToViewport();
}

void UWidgetsPresenterComponent::_createInventoryPresenter(const TWeakObjectPtr<APlayerController>& ownerPlayerC) {
    check(ownerPlayerC.IsValid());
    check(IsValid(_inventoryPresenterWidgetClass));

    _inventoryPresenterWidget = CreateWidget<UInventoryPresenterWidget>(ownerPlayerC.Get(), _inventoryPresenterWidgetClass);
}
