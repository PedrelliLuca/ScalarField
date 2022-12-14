// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryLookupState.h"

#include "IdleState.h"
#include "InventoryComponent.h"
#include "MovementCommandSetter.h"
#include "WidgetsPresenterComponent.h"

TObjectPtr<USkillUserState> UInventoryLookupState::OnToggleInventory(const TObjectPtr<AController> controller) {
	return NewObject<UIdleState>(controller, UIdleState::StaticClass());
}

void UInventoryLookupState::OnEnter(TObjectPtr<AController> controller) {
	const TWeakObjectPtr<UWidgetsPresenterComponent> widgetsPresenter = controller->FindComponentByClass<UWidgetsPresenterComponent>();
	// How did you even get in this state if the controller cannot present widgets?
	check(widgetsPresenter.IsValid());
	// When you enter this state the inventory shouldn't be on the viewport, it's this state's responsibility to add it
	check(!widgetsPresenter->IsInventoryOnViewport());
	
	const auto pawn = controller->GetPawn();
	check(IsValid(pawn));
	const TWeakObjectPtr<UInventoryComponent> inventory = pawn->FindComponentByClass<UInventoryComponent>();
	// Controllers with widget presenters must also have inventories to toggle
	check(inventory.IsValid());
	widgetsPresenter->ShowInventory(inventory);
	
	const auto movementSetters = controller->GetComponentsByInterface(UMovementCommandSetter::StaticClass());
	check(movementSetters.Num() == 1);
	const auto movementSetter = Cast<IMovementCommandSetter>(movementSetters[0]);
	check(movementSetter != nullptr);
	
	// Pawns cannot move while the inventory widget is open
	movementSetter->SetMovementMode(EMovementCommandMode::MCM_Still);
}

void UInventoryLookupState::OnLeave(TObjectPtr<AController> controller) {
	const TWeakObjectPtr<UWidgetsPresenterComponent> widgetsPresenter = controller->FindComponentByClass<UWidgetsPresenterComponent>();
	// How did you even get in this state if the controller cannot present widgets?
	check(widgetsPresenter.IsValid())
	check(widgetsPresenter->IsInventoryOnViewport());
	widgetsPresenter->HideInventory();
}
