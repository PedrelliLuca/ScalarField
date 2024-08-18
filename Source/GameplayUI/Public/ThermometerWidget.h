// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "PawnBindableWidgetInterface.h"

#include "ThermometerWidget.generated.h"

class UTemperatureDamageHandlerComponent;
class UThermodynamicsInteractorComponent;
class UThermodynamicsPresenterComponent;

/**
 *
 */
UCLASS(Blueprintable)
class GAMEPLAYUI_API UThermometerWidget : public UUserWidget, public IPawnBindableWidget {
    GENERATED_BODY()

public:
    void SetPawn(TWeakObjectPtr<APawn> pawn) override;
    void ForgetCurrentPawn() override;
    void Show() override;
    void Hide() override;

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void _setTemperature(float temperature, FLinearColor temperatureColor);

    UFUNCTION(BlueprintImplementableEvent)
    void _setComfortInterval(float minComfortTemperature, float maxComfortTemperature);

private:
    void _bindAll();
    void _unbindAll();

    void _onTemperatureChange(float newTemperture);

    FDelegateHandle _temperatureChangedHandle;

    TWeakObjectPtr<UThermodynamicsInteractorComponent> _thermoIntC = nullptr;
    TWeakObjectPtr<UThermodynamicsPresenterComponent> _thermoPresC = nullptr;
    TWeakObjectPtr<UTemperatureDamageHandlerComponent> _tempDmgHandlerC = nullptr;
};
