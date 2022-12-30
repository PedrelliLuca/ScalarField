// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemContainerWidgetInterface.h"

#include "NewInventoryWidget.generated.h"

UCLASS()
class INVENTORYPRESENTER_API UNewInventoryWidget : public UUserWidget, public IItemContainerWidget {
     GENERATED_BODY()
  
public:
     void SetInventory(TWeakInterfacePtr<IInventory> inventory) override;
};
