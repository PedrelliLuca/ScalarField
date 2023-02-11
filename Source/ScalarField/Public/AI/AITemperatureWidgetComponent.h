// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
};
