// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItemInterface.h"
#include "UObject/WeakInterfacePtr.h"

#include "ItemWidgetInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemUsage, TWeakInterfacePtr<IItem>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemDiscarded, TWeakInterfacePtr<IItem>);

UINTERFACE(MinimalAPI, NotBlueprintable)
class UItemWidget : public UInterface {
	GENERATED_BODY()
};

/* Interface representing an inventory. */
class IItemWidget {
	GENERATED_BODY()

public:
	virtual void SetItem(TWeakInterfacePtr<IItem> item) = 0;
	
	FOnItemUsage& OnItemUsage() { return _onItemUsage; }
	FOnItemDiscarded& OnItemDiscarded() { return _onItemDiscarded; }

private:
	FOnItemUsage _onItemUsage{};
	FOnItemDiscarded _onItemDiscarded{};
};
