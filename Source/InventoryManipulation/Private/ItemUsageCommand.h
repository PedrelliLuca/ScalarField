// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "InventoryInterface.h"
#include "PauseCommandInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "ItemUsageCommand.generated.h"

UCLASS()
class INVENTORYMANIPULATION_API UItemUsageCommand : public UObject, public IPauseCommand {
     GENERATED_BODY()

     friend class UItemUsageCommandFactory;
     
public:
     void Execute() override;

private:
     TWeakInterfacePtr<IItem> _item = nullptr;
     TWeakInterfacePtr<IInventory> _inventory = nullptr;
     int32 _quantity = 0;
};