// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GAMEPLAYUI_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void SetMaxHealth(double newMaxHealth);
	void SetCurrentHealth(double newCurrentHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void SetHealth();
	UFUNCTION(BlueprintImplementableEvent)
	void SetHealthRegen(double newHealthRegen);

	void SetMaxMana(double newMaxMana);
	void SetCurrentMana(double newCurrentMana);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMana();
	UFUNCTION(BlueprintImplementableEvent)
	void SetManaRegen(double newManaRegen);

	UFUNCTION(BlueprintImplementableEvent)
	void SetTemperature(double temperature, FLinearColor temperatureColor);

	UFUNCTION(BlueprintImplementableEvent)
	void SetPauseStatus(bool bIsGamePaused);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	double _currentHealth = 0.;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	double _maxHealth = 0.;

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	double _currentMana = 0.;
	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	double _maxMana = 0.;
};
