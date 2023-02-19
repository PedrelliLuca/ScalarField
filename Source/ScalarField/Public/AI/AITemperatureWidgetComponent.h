// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AI/AITemperatureWidget.h"
#include "Components/WidgetComponent.h"

#include "AITemperatureWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class SCALARFIELD_API UAITemperatureWidgetComponent : public UWidgetComponent {
	GENERATED_BODY()
	
public:
	UAITemperatureWidgetComponent();

protected:
	void BeginPlay() override;

private:
	void _bindTemperatureComponent(UAITemperatureWidget* temperatureWidget) const;
	void _bindHealthComponent(UAITemperatureWidget* temperatureWidget) const;
	void _bindManaComponent(UAITemperatureWidget* temperatureWidget) const;
};
