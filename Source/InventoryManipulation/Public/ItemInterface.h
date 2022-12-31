// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UItem : public UInterface {
	GENERATED_BODY()
};

/* Interface representing an inventory. */
class IItem {
	GENERATED_BODY()

public:
	virtual void Use(TWeakObjectPtr<AActor> actor) = 0;

	virtual int32 GetMaxQuantity() const = 0;
	virtual int32 GetQuantity() const = 0;
	virtual void SetQuantity(int32 newQuantity) = 0;
	virtual bool IsStackable() const = 0;
	virtual bool ShouldShowInInventory() const = 0;
	virtual const FText& GetNameText() const = 0;
	virtual TObjectPtr<UTexture2D> GetThumbnail() = 0;
};
