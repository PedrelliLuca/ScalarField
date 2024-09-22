// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupSpawnController.h"

#include "PickupInterface.h"
#include "PickupSpawnSettings.h"
#include "TacticalPauseWorldSubsystem.h"

UPickupSpawnController::UPickupSpawnController() {
    auto const pickupSpawnSettings = GetDefault<UPickupSpawnSettings>();
    _pickupClass = pickupSpawnSettings->GetPickupClass();
    check(IsValid(_pickupClass));

    _pickupSpawnCmdFactory = CreateDefaultSubobject<UPickupSpawnCommandFactory>(TEXT("Pickup Spawn Command Factory"));
    _pickupSpawnCmdFactory->SetPickupClass(_pickupClass);
}

void UPickupSpawnController::SetItemDropNotifier(TWeakInterfacePtr<IInventoryContainerWidget> itemDropNotifier) {
    if (_itemDropNotifier.IsValid()) {
        _itemDropNotifier->OnItemFromInventoryBeingDropped().Remove(_itemDropHandle);
        _itemDropHandle.Reset();
    }

    check(itemDropNotifier.IsValid());
    _itemDropNotifier = MoveTemp(itemDropNotifier);
}

void UPickupSpawnController::SetPickupTransformCallbackForUIDrop(TFunction<FTransform()>&& pickupTransformCallback) {
    pickupTransformCallback.CheckCallable();
    _getPickupTransformUICallback = MoveTemp(pickupTransformCallback);
    _pickupSpawnCmdFactory->SetPickupSpawnLocationCallback(_getPickupTransformUICallback);
}

void UPickupSpawnController::SetPickupTransformCallbackForDeathDrop(TFunction<FTransform(TObjectPtr<AActor>)>&& pickupTransformCallback) {
    pickupTransformCallback.CheckCallable();
    _getPickupTransformDeathCallback = MoveTemp(pickupTransformCallback);
}

void UPickupSpawnController::BindPickupSpawn() {
    check(!_itemDropHandle.IsValid() && _itemDropNotifier.IsValid());
    _itemDropHandle = _itemDropNotifier->OnItemFromInventoryBeingDropped().AddUObject(this, &UPickupSpawnController::_spawnPickup);
}

void UPickupSpawnController::UnbindPickupSpawn() {
    check(_itemDropNotifier.IsValid());
    _itemDropNotifier->OnItemFromInventoryBeingDropped().Remove(_itemDropHandle);
    _itemDropHandle.Reset();
}

void UPickupSpawnController::BindPickupsDropAtDeath(TObjectPtr<AActor> const actor) {
    auto const healthC = actor->FindComponentByClass<UHealthComponent>();
    if (!ensureMsgf(IsValid(healthC), TEXT("%s(): Can't bind death pickup drop for actor, no UHealthComponent"), *FString{__FUNCTION__})) {
        return;
    }

    TWeakInterfacePtr<IInventory> const inventoryC = actor->FindComponentByInterface<UInventory>();
    if (!ensureMsgf(inventoryC.IsValid(), TEXT("%s(): Can't bind death pickup drop for actor, no IInventory"), *FString{__FUNCTION__})) {
        return;
    }

    auto handle = healthC->OnDeath().AddUObject(this, &UPickupSpawnController::_dropPickupsOnDeath);

    auto deathDropParams = FDeathDropParams{healthC, inventoryC, MoveTemp(handle)};
    _actorToDeathDropParams.Emplace(actor, MoveTemp(deathDropParams));
}

void UPickupSpawnController::_spawnPickup(TWeakInterfacePtr<IItem> item, int32 const quantity, TWeakInterfacePtr<IInventory> inventory) {
    check(item.IsValid() && inventory.IsValid());
    auto const pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
    if (!pauseSubsys->IsTacticalPauseOn()) {
        int32 const droppedQuantity = inventory->ConsumeItem(item.Get(), quantity);

        FActorSpawnParameters spawnParams{};
        spawnParams.bNoFail = true;
        spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FTransform const spawnTransform = _getPickupTransformUICallback();

        check(IsValid(_pickupClass));

        TWeakInterfacePtr<IPickup> const pickup = GetWorld()->SpawnActor<AActor>(_pickupClass, spawnTransform, spawnParams);
        check(pickup.IsValid());

        pickup->InitializePickup(item.GetObject()->GetClass(), droppedQuantity);
    } else {
        check(IsValid(_pickupSpawnCmdFactory));
        _pickupSpawnCmdFactory->SetCommandItem(MoveTemp(item));
        _pickupSpawnCmdFactory->SetCommandInventory(MoveTemp(inventory));
        _pickupSpawnCmdFactory->SetCommandQuantity(quantity);
        pauseSubsys->SetPauseOffCommand(_pickupSpawnCmdFactory->CreateCommand());
    }
}

void UPickupSpawnController::_dropPickupsOnDeath(TObjectPtr<AActor> const deadActor) {
    check(IsValid(deadActor));
    auto const& deathDropParams = _actorToDeathDropParams.FindChecked(deadActor);

    auto const& inventory = deathDropParams.InventoryToDrop;
    check(inventory.IsValid());

    // Drop every single item in the inventory.
    for (auto const& item : inventory->GetItems()) {
        int32 const droppedQuantity = inventory->ConsumeItem(item.Get(), item->GetQuantity());

        FActorSpawnParameters spawnParams{};
        spawnParams.bNoFail = true;
        spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FTransform const spawnTransform = _getPickupTransformDeathCallback(deadActor);

        check(IsValid(_pickupClass));

        TWeakInterfacePtr<IPickup> const pickup = GetWorld()->SpawnActor<AActor>(_pickupClass, spawnTransform, spawnParams);
        check(pickup.IsValid());

        pickup->InitializePickup(item.GetObject()->GetClass(), droppedQuantity);
    }

    // Cache cleanup.
    deathDropParams.DeadHealthC->OnDeath().Remove(deathDropParams.OnDeathHandle);
    _actorToDeathDropParams.Remove(deadActor);
}
