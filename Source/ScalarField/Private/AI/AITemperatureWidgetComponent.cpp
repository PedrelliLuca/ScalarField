// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AITemperatureWidgetComponent.h"

#include "AI/AITemperatureWidget.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicComponent.h"

UAITemperatureWidgetComponent::UAITemperatureWidgetComponent() {
	Space = EWidgetSpace::Screen;
	DrawSize = FIntPoint(200, 50);
	bDrawAtDesiredSize = true;

	SetActive(true);
}

void UAITemperatureWidgetComponent::BeginPlay() {
	Super::BeginPlay();

	const auto owner = GetOwner();
	if (const auto thermoC = owner->FindComponentByClass<UThermodynamicComponent>(); IsValid(thermoC)) {
		if (const auto aiTemperatureWidget = Cast<UAITemperatureWidget>(GetUserWidgetObject()); IsValid(aiTemperatureWidget)) {
			if (const auto temperatureDmgC = owner->FindComponentByClass<UTemperatureDamageHandlerComponent>(); IsValid(temperatureDmgC)) {
				aiTemperatureWidget->SetComfortIntervalLimits(temperatureDmgC->GetMaxComfortTemperature(), temperatureDmgC->GetMinComfortTemperature());
		    }

			thermoC->OnTemperatureChanged.AddUObject(aiTemperatureWidget, &UAITemperatureWidget::OnTemperatureUpdate);
		} else {
			UE_LOG(LogTemp, Error, TEXT("%s(): Invalid User Widget, must be an AITemperatureWidget"), *FString{ __FUNCTION__ });
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI Temperature Widget Component on APawn without Thermodynamic Component"), *FString{ __FUNCTION__ });
	}
}
