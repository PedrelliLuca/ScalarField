// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"

#include "QuantitySetterWidget.generated.h"

UCLASS()
class INVENTORYPRESENTER_API UQuantitySetterWidget : public UUserWidget {
     GENERATED_BODY()

public:
     TWeakObjectPtr<UEditableTextBox> GetQuantityTextBox() { return _quantityTextBox; }

private:
     
     UPROPERTY(meta = (BindWidget))
     TObjectPtr<UEditableTextBox> _quantityTextBox;
};

