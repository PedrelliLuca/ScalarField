// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AIStatusWidgetComponent.h"

#include "HealthComponent.h"
#include "ManaComponent.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicComponent.h"

UAIStatusWidgetComponent::UAIStatusWidgetComponent() {
    Space = EWidgetSpace::Screen;
    DrawSize = FIntPoint(200, 50);
    bDrawAtDesiredSize = true;

    SetActive(true);
}

void UAIStatusWidgetComponent::BeginPlay() {
    Super::BeginPlay();

    if (const auto aiStatusWidget = Cast<UAIStatusWidget>(GetUserWidgetObject()); IsValid(aiStatusWidget)) {
        _bindTemperatureComponent(aiStatusWidget);
        _bindHealthComponent(aiStatusWidget);
        _bindManaComponent(aiStatusWidget);
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): Invalid User Widget, must be an AITemperatureWidget"), *FString{__FUNCTION__});
    }
}

void UAIStatusWidgetComponent::_bindTemperatureComponent(UAIStatusWidget* const statusWidget) const {
    const auto owner = GetOwner();

    if (const auto thermoC = owner->FindComponentByClass<UThermodynamicComponent>(); IsValid(thermoC)) {
        if (const auto temperatureDmgC = owner->FindComponentByClass<UTemperatureDamageHandlerComponent>(); IsValid(temperatureDmgC)) {
            statusWidget->SetComfortIntervalLimits(temperatureDmgC->GetMaxComfortTemperature(), temperatureDmgC->GetMinComfortTemperature());
        }

        thermoC->OnTemperatureChanged.AddUObject(statusWidget, &UAIStatusWidget::OnTemperatureUpdate);
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI Temperature Widget Component on AActor without Thermodynamic Component"), *FString{__FUNCTION__});
    }
}

void UAIStatusWidgetComponent::_bindHealthComponent(UAIStatusWidget* const statusWidget) const {
    const auto owner = GetOwner();

    if (const auto healthC = owner->FindComponentByClass<UHealthComponent>(); IsValid(healthC)) {
        statusWidget->OnHealthUpdate(healthC->GetCurrentHealth());
        statusWidget->OnMaxHealthUpdate(healthC->GetMaxHealth());
        healthC->OnHealthChanged().AddUObject(statusWidget, &UAIStatusWidget::OnHealthUpdate);
        healthC->OnMaxHealthChanged().AddUObject(statusWidget, &UAIStatusWidget::OnMaxHealthUpdate);
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI Temperature Widget Component on APawn without Health Component"), *FString{__FUNCTION__});
    }
}

void UAIStatusWidgetComponent::_bindManaComponent(UAIStatusWidget* const statusWidget) const {
    const auto owner = GetOwner();

    if (const auto manaC = owner->FindComponentByClass<UManaComponent>(); IsValid(manaC)) {
        statusWidget->OnManaUpdate(manaC->GetCurrentMana());
        statusWidget->OnMaxManaUpdate(manaC->GetMaxMana());
        manaC->OnManaChanged().AddUObject(statusWidget, &UAIStatusWidget::OnManaUpdate);
        manaC->OnMaxManaChanged().AddUObject(statusWidget, &UAIStatusWidget::OnMaxManaUpdate);
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s(): AI Temperature Widget Component on APawn without Mana Component"), *FString{__FUNCTION__});
    }
}
