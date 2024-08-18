// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermometerWidget.h"

#include "ThermodynamicsInteractorComponent.h"
#include "ThermodynamicsPresenterComponent.h"

void UThermometerWidget::SetPawn(TWeakObjectPtr<APawn> const pawn) {
    _thermoIntC = pawn->FindComponentByClass<UThermodynamicsInteractorComponent>();
    if (!_thermoIntC.IsValid()) {
        UE_LOG(LogTemp, Error, TEXT(__FUNCTION__": missing UThermodynamicsInteractorComponent"));
    }

    _thermoPresC = pawn->FindComponentByClass<UThermodynamicsPresenterComponent>();
    if (!_thermoPresC.IsValid()) {
        UE_LOG(LogTemp, Error, TEXT(__FUNCTION__": missing UThermodynamicsPresenterComponent"));
    }
}

void UThermometerWidget::ForgetCurrentPawn() {
    _unbindAll();
    _thermoIntC = nullptr;
    _thermoPresC = nullptr;
}

void UThermometerWidget::Show() {
    _bindAll();
}

void UThermometerWidget::Hide() {
    _unbindAll();
}

void UThermometerWidget::_bindAll() {
    if (_thermoIntC.IsValid()) {
        _onTemperatureChange(_thermoIntC->GetTemperature());

        _temperatureChangedHandle = _thermoIntC->OnTemperatureChanged.AddUObject(this, &UThermometerWidget::_onTemperatureChange);
    }
}

void UThermometerWidget::_unbindAll() {
    if (_thermoIntC.IsValid()) {
        _thermoIntC->OnTemperatureChanged.Remove(_temperatureChangedHandle);
    }
}

void UThermometerWidget::_onTemperatureChange(const float newTemperture) {
    auto temperatureColor = FLinearColor::Black;

    if (_thermoPresC.IsValid()) {
        temperatureColor = _thermoPresC->GetTemperatureColor();
    }
    _setTemperature(newTemperture, temperatureColor);
}
