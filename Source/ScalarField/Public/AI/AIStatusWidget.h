// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "AIStatusWidget.generated.h"

/**
 *
 */
UCLASS()
class SCALARFIELD_API UAIStatusWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void SetComfortIntervalLimits(float upperLimit, float lowerLimit);

    void OnTemperatureUpdate(double newTemperature);

    void OnHealthUpdate(float newHealth);
    void OnMaxHealthUpdate(float newMaxHealth);

    void OnManaUpdate(double newMana);
    void OnMaxManaUpdate(double newMaxMana);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void _updateThermometer(double newTemperature);

    UFUNCTION(BlueprintImplementableEvent)
    void _updateHealth();

    UFUNCTION(BlueprintImplementableEvent)
    void _updateMana();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comfort Interval")
    float _comfortUpperLimit = -1.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comfort Interval")
    float _comfortLowerLimit = -1.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    float _currentHealth = 0.0f;
    UPROPERTY(BlueprintReadOnly, Category = "Health")
    float _maxHealth = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Mana")
    float _currentMana = 0.0f;
    UPROPERTY(BlueprintReadOnly, Category = "Mana")
    float _maxMana = 0.0f;
};
