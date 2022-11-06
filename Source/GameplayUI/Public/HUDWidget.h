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

	/*void SetMaxHealth(float NewMaxHealth);

	void SetCurrentHealth(float NewCurrentHealth);

	void SetHealthPercentage(float NewHealthPercentage);

	void SetHealthRegenRate(float NewHealthRegenRate);

	void SetMaxStamina(float NewMaxStamina);

	void SetCurrentStamina(float NewCurrentStamina);	

	void SetStaminaPercentage(float NewStaminaPercentage);

	void SetStaminaRegenRate(float NewStaminaRegenRate);

	void SetMaxMana(float NewMaxMana);

	void SetCurrentMana(float NewCurrentMana);

	void SetManaPercentage(float NewManaPercentage);

	void SetManaRegenRate(float NewManaRegenRate);

	void ShowAbilityConfirmCancelText(bool ShowText);

	void SetExperience(int32 NewExperience);

	void SetHeroLevel(int32 NewHeroLevel);

	void SetGold(int32 NewGold);

	void SetMeteorCountdown(float TimeRemaining);

	void EndMeteorCountdown();

	void UpdateMeteorCountdown();

	void UpdateArmorStacks(int32 NewStackCount);

private:
	float CurrentHealth;
	float MaxHealth;

	void SetHealth();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Health;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_HealthRegen;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Health;

	float CurrentStamina;
	float MaxStamina;

	void SetStamina();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Stamina;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_StaminaRegen;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Stamina;

	float CurrentMana;
	float MaxMana;

	void SetMana();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Mana;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ManaRegen;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ProgressBar_Mana;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_XP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_Gold;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ConfirmCancel;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_CooldownTimeMeteor;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Meteor;

	float MeteorCooldownTimeRemaining;
	FTimerHandle MeteorCooldownTimerHandle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextBlock_ArmorStacks;*/
};
