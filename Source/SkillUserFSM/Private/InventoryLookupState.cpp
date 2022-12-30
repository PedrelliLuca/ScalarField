// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryLookupState.h"

#include "IdleState.h"
#include "InventoryManipulationSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "MovementCommandSetter.h"
#include "StateComponent.h"

TObjectPtr<USkillUserState> UInventoryLookupState::OnToggleInventory(const TObjectPtr<AController> controller) {
	return NewObject<UIdleState>(controller, UIdleState::StaticClass());
}

void UInventoryLookupState::OnEnter(TObjectPtr<AController> controller) {
	// const auto inventorySubsys = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInventorySubsystem>();
	// inventorySubsys->ShowInventoryOfActor(controller->GetPawn());
	//
	// const TWeakObjectPtr<UStateComponent> stateC = controller->FindComponentByClass<UStateComponent>();
	// check(stateC.IsValid());
	// _widgetClosedHandle = inventorySubsys->OnInventoryWidgetClosed().AddUObject(stateC.Get(), &UStateComponent::PerformInventoryToggleBehavior);

	const auto inventorySubsys = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInventoryManipulationSubsystem>();
	inventorySubsys->OpenInventoryOfActor(controller->GetPawn());

	const TWeakObjectPtr<UStateComponent> stateC = controller->FindComponentByClass<UStateComponent>();
	check(stateC.IsValid());
	_inventoryWidgetCloseHandle = inventorySubsys->OnInventoryClosedFromUI().AddUObject(stateC.Get(), &UStateComponent::PerformInventoryToggleBehavior);
	
	const auto movementSetters = controller->GetComponentsByInterface(UMovementCommandSetter::StaticClass());
	check(movementSetters.Num() == 1);
	const auto movementSetter = Cast<IMovementCommandSetter>(movementSetters[0]);
	check(movementSetter != nullptr);
	
	// Pawns cannot move while the inventory widget is open
	movementSetter->SetMovementMode(EMovementCommandMode::MCM_Still);
}

void UInventoryLookupState::OnLeave(TObjectPtr<AController> controller) {
	// const TWeakObjectPtr<UWidgetsPresenterComponent> widgetsPresenter = controller->FindComponentByClass<UWidgetsPresenterComponent>();
	// // How did you even get in this state if the controller cannot present widgets?
	// check(widgetsPresenter.IsValid())
	// check(widgetsPresenter->IsInventoryOnViewport());
	// widgetsPresenter->HideInventory();

	// const auto inventorySubsys = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInventorySubsystem>();
	// inventorySubsys->OnInventoryWidgetClosed().Remove(_inventoryWidgetCloseHandle);
	
	const auto inventorySubsys = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInventoryManipulationSubsystem>();
	inventorySubsys->CloseInventory();
	inventorySubsys->OnInventoryClosedFromUI().Remove(_inventoryWidgetCloseHandle);
}
