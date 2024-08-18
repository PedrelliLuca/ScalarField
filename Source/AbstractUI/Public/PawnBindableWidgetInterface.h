// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PawnBindableWidgetInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UPawnBindableWidget : public UInterface {
    GENERATED_BODY()
};

/* Interface for widgets that need to read data from a single pawn via some delegates */
class IPawnBindableWidget {
    GENERATED_BODY()

public:
    // TODO: why did I link this interface to pawns when I created it? Why not SetActor()? Weird...
    virtual void SetPawn(TWeakObjectPtr<APawn> pawn) = 0;
    virtual void ForgetCurrentPawn() = 0;
    virtual void Show() = 0;
    virtual void Hide() = 0;
};
