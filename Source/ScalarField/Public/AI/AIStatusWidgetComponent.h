// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AI/AIStatusWidget.h"
#include "Components/WidgetComponent.h"

#include "AIStatusWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class SCALARFIELD_API UAIStatusWidgetComponent : public UWidgetComponent {
	GENERATED_BODY()
	
public:
	UAIStatusWidgetComponent();

protected:
	void BeginPlay() override;

private:
	void _bindTemperatureComponent(UAIStatusWidget* statusWidget) const;
	void _bindHealthComponent(UAIStatusWidget* statusWidget) const;
	void _bindManaComponent(UAIStatusWidget* statusWidget) const;
};
