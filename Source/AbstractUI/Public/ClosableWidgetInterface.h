// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ClosableWidgetInterface.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCloseFromUI);

UINTERFACE(MinimalAPI, NotBlueprintable)
class UClosableWidget : public UInterface {
    GENERATED_BODY()
};

/* Interface for widgets that can be closed by using some UI element, like a button. */
class IClosableWidget {
    GENERATED_BODY()

public:
    FOnCloseFromUI& OnCloseFromUI() { return _onCloseFromUI; }

private:
    FOnCloseFromUI _onCloseFromUI{};
};
