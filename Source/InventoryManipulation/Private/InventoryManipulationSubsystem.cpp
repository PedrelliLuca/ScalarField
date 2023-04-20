// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryManipulationSubsystem.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void UInventoryManipulationSubsystem::Initialize(FSubsystemCollectionBase& collection) {
    Super::Initialize(collection);

    _inventoryToggleController = NewObject<UInventoryToggleController>(this, UInventoryToggleController::StaticClass());
    _itemUsageController = NewObject<UItemUsageController>(this, UItemUsageController::StaticClass());
    _pickupSpawnController = NewObject<UPickupSpawnController>(this, UPickupSpawnController::StaticClass());

    // TODO: this should only be a default value, this class should provide an API for this callback to be decided by the users.
    _pickupSpawnController->SetPickupTransformCallbackForDeathDrop([](const TObjectPtr<AActor> deadActor) {
        check(IsValid(deadActor));
        auto transform = deadActor->GetActorTransform();

        if (const auto deadCharacter = Cast<ACharacter>(deadActor)) {
            FVector location = deadCharacter->GetActorLocation();
            location.Z -= deadCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
            transform.SetLocation(MoveTemp(location));
        }

        return transform;
    });
}

void UInventoryManipulationSubsystem::SetInventoryContainerWidget(TWeakInterfacePtr<IInventoryContainerWidget> inventoryContainer) {
    _inventoryToggleController->SetInventoryContainerWidget(inventoryContainer);
    _itemUsageController->SetItemUsageNotifier(inventoryContainer);
    _pickupSpawnController->SetItemDropNotifier(inventoryContainer);
}

void UInventoryManipulationSubsystem::SetHUDToShowOnClose(TWeakInterfacePtr<IPawnBindableWidget> widgetOnClose) {
    _inventoryToggleController->SetHUDToShowOnClose(MoveTemp(widgetOnClose));
}

void UInventoryManipulationSubsystem::OpenInventoryOfActor(TWeakObjectPtr<AActor> actor) {
    _inventoryToggleController->OpenInventoryOfActor(MoveTemp(actor));
    _itemUsageController->BindItemUsage();
    _pickupSpawnController->SetPickupTransformCallbackForUIDrop([actor]() {
        check(actor.IsValid());
        FTransform transform = actor->GetActorTransform();

        if (const auto character = Cast<ACharacter>(actor)) {
            FVector location = character->GetActorLocation();
            location.Z -= character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
            transform.SetLocation(MoveTemp(location));
        }

        return transform;
    });
    _pickupSpawnController->BindPickupSpawn();
}

void UInventoryManipulationSubsystem::CloseInventory() {
    _inventoryToggleController->CloseInventory();
    _itemUsageController->UnbindItemUsage();
    _pickupSpawnController->UnbindPickupSpawn();
}

void UInventoryManipulationSubsystem::SetupDeathDropForActor(TObjectPtr<AActor> actor) {
    _pickupSpawnController->BindPickupsDropAtDeath(MoveTemp(actor));
}
