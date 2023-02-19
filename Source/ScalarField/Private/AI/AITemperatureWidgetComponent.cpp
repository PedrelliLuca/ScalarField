// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AITemperatureWidgetComponent.h"

#include "HealthComponent.h"
#include "ManaComponent.h"
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
	
	if (const auto aiTemperatureWidget = Cast<UAITemperatureWidget>(GetUserWidgetObject()); IsValid(aiTemperatureWidget)) {
		_bindTemperatureComponent(aiTemperatureWidget);
		_bindHealthComponent(aiTemperatureWidget);
		_bindManaComponent(aiTemperatureWidget);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s(): Invalid User Widget, must be an AITemperatureWidget"), *FString{ __FUNCTION__ });
	}
}

void UAITemperatureWidgetComponent::_bindTemperatureComponent(UAITemperatureWidget* const temperatureWidget) const {
	const auto owner = GetOwner();

	if (const auto thermoC = owner->FindComponentByClass<UThermodynamicComponent>(); IsValid(thermoC)) {
		if (const auto temperatureDmgC = owner->FindComponentByClass<UTemperatureDamageHandlerComponent>(); IsValid(temperatureDmgC)) {
			temperatureWidget->SetComfortIntervalLimits(temperatureDmgC->GetMaxComfortTemperature(), temperatureDmgC->GetMinComfortTemperature());
		}

		thermoC->OnTemperatureChanged.AddUObject(temperatureWidget, &UAITemperatureWidget::OnTemperatureUpdate);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI Temperature Widget Component on AActor without Thermodynamic Component"), *FString{ __FUNCTION__ });
	}
}

void UAITemperatureWidgetComponent::_bindHealthComponent(UAITemperatureWidget* const temperatureWidget) const {
	const auto owner = GetOwner();

	if (const auto healthC = owner->FindComponentByClass<UHealthComponent>(); IsValid(healthC)) {
		temperatureWidget->OnHealthUpdate(healthC->GetCurrentHealth());
		temperatureWidget->OnMaxHealthUpdate(healthC->GetMaxHealth());
		healthC->OnHealthChanged().AddUObject(temperatureWidget, &UAITemperatureWidget::OnHealthUpdate);
		healthC->OnMaxHealthChanged().AddUObject(temperatureWidget, &UAITemperatureWidget::OnMaxHealthUpdate);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI Temperature Widget Component on APawn without Health Component"), *FString{ __FUNCTION__ });
	}
}

void UAITemperatureWidgetComponent::_bindManaComponent(UAITemperatureWidget* const temperatureWidget) const {
	const auto owner = GetOwner();

	if (const auto manaC = owner->FindComponentByClass<UManaComponent>(); IsValid(manaC)) {
		temperatureWidget->OnManaUpdate(manaC->GetCurrentMana());
		temperatureWidget->OnMaxManaUpdate(manaC->GetMaxMana());
		manaC->OnManaChanged().AddUObject(temperatureWidget, &UAITemperatureWidget::OnManaUpdate);
		manaC->OnMaxManaChanged().AddUObject(temperatureWidget, &UAITemperatureWidget::OnMaxManaUpdate);
	} else {
		UE_LOG(LogTemp, Error, TEXT("%s(): AI Temperature Widget Component on APawn without Mana Component"), *FString{ __FUNCTION__ });
	}
}
