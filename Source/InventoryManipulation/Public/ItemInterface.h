// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemInterface.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnItemModified);

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
    virtual double GetWeight() const = 0;
    virtual double GetStackWeight() const = 0;
    virtual double GetVolume() const = 0;
    virtual double GetStackVolume() const = 0;
    virtual const FText& GetNameText() const = 0;
    virtual TObjectPtr<UTexture2D> GetThumbnail() = 0;
    virtual TObjectPtr<UStaticMesh> GetMesh() = 0;

    virtual void SetQuantity(int32 newQuantity) = 0;

    virtual bool IsStackable() const = 0;
    virtual bool ShouldShowInInventory() const = 0;
    virtual bool DoesUseConsume() const = 0;

    FOnItemModified& OnItemModified() { return _onItemModified; }

private:
    FOnItemModified _onItemModified;
};
