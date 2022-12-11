// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

class UHealthComponent;
class UManaComponent;
class UThermodynamicComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class GAMEPLAYUI_API UHUDWidget : public UUserWidget {
	GENERATED_BODY()
	
public:
	void BindToPawn(TWeakObjectPtr<APawn> pawn);
	void RemovePawnBindings();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void _setHealth();
	UFUNCTION(BlueprintImplementableEvent)
	void _setHealthRegen(double newHealthRegen);

	UFUNCTION(BlueprintImplementableEvent)
	void _setMana();
	UFUNCTION(BlueprintImplementableEvent)
	void _setManaRegen(double newManaRegen);

	UFUNCTION(BlueprintImplementableEvent)
	void _setTemperature(double temperature, FLinearColor temperatureColor);

	UFUNCTION(BlueprintImplementableEvent)
	void _setPauseStatus(bool bIsGamePaused);
	
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	double _currentHealth = 0.;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	double _maxHealth = 0.;

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	double _currentMana = 0.;
	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	double _maxMana = 0.;
	
private:
	void _setMaxHealth(double newMaxHealth);
	void _setCurrentHealth(double newCurrentHealth);

	void _setMaxMana(double newMaxMana);
	void _setCurrentMana(double newCurrentMana);
	
	void _onTemperatureChange(double newTemperture);

	void _onTacticalPauseToggle(bool bIsTacticalPauseOn, double);

	FDelegateHandle _healthChangedHandle;
	FDelegateHandle _maxHealthChangedHandle;
	FDelegateHandle _healthRegenChangedHandle;
	FDelegateHandle _manaChangedHandle;
	FDelegateHandle _maxManaChangedHandle;
	FDelegateHandle _manaRegenChangedHandle;
	FDelegateHandle _temperatureChangedHandle;
	FDelegateHandle _pauseToggleHandle;

	TWeakObjectPtr<UHealthComponent> _healthC = nullptr;
	TWeakObjectPtr<UManaComponent> _manaC = nullptr;
	TWeakObjectPtr<UThermodynamicComponent> _thermoC = nullptr;
};
