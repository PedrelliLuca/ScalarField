// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
//#include "Components/Image.h"
//#include "Components/ProgressBar.h"
//#include "Components/TextBlock.h"
//#include "Kismet/KismetTextLibrary.h"
//#include "TimerManager.h"
//
//
//void UGASHUDWidget::SetMaxHealth(float NewMaxHealth) 
//{
//    MaxHealth = NewMaxHealth;
//    SetHealth();
//}
//
//void UGASHUDWidget::SetCurrentHealth(float NewCurrentHealth) 
//{
//    CurrentHealth = NewCurrentHealth;
//    SetHealth();
//}
//
//void UGASHUDWidget::SetHealthPercentage(float NewHealthPercentage) 
//{
//    ProgressBar_Health->SetPercent(NewHealthPercentage);
//}
//
//void UGASHUDWidget::SetHealthRegenRate(float NewHealthRegenRate) 
//{
//    FText HealthRegenRateText = UKismetTextLibrary::Conv_FloatToText(NewHealthRegenRate, ERoundingMode::HalfToEven, false, true, 1, 4, 1, 1);
//    FFormatNamedArguments Args;
//    Args.Add("Regen", HealthRegenRateText);
//    FText OutputText = FText::Format(FTextFormat::FromString(TEXT("+ {Regen}")), Args);
//    TextBlock_HealthRegen->SetText(OutputText);
//}
//
//void UGASHUDWidget::SetMaxStamina(float NewMaxStamina) 
//{
//    MaxStamina = NewMaxStamina;
//    SetStamina();
//}
//
//void UGASHUDWidget::SetCurrentStamina(float NewCurrentStamina) 
//{
//    CurrentStamina = NewCurrentStamina;
//    SetStamina();
//}
//
//void UGASHUDWidget::SetStaminaPercentage(float NewStaminaPercentage) 
//{
//    ProgressBar_Stamina->SetPercent(NewStaminaPercentage);
//}
//
//void UGASHUDWidget::SetStaminaRegenRate(float NewStaminaRegenRate) 
//{
//    FText StaminaRegenRateText = UKismetTextLibrary::Conv_FloatToText(NewStaminaRegenRate, ERoundingMode::HalfToEven, false, true, 1, 4, 1, 1);
//    FFormatNamedArguments Args;
//    Args.Add("Regen", StaminaRegenRateText);
//    FText OutputText = FText::Format(FTextFormat::FromString(TEXT("+ {Regen}")), Args);
//    TextBlock_StaminaRegen->SetText(OutputText);
//}
//
//void UGASHUDWidget::SetMaxMana(float NewMaxMana) 
//{
//    MaxMana = NewMaxMana;
//    SetMana();
//}
//
//void UGASHUDWidget::SetCurrentMana(float NewCurrentMana) 
//{
//    CurrentMana = NewCurrentMana;
//    SetMana();
//}
//
//void UGASHUDWidget::SetManaPercentage(float NewManaPercentage) 
//{
//    ProgressBar_Mana->SetPercent(NewManaPercentage);
//}
//
//void UGASHUDWidget::SetManaRegenRate(float NewManaRegenRate) 
//{
//    FText ManaRegenRateText = UKismetTextLibrary::Conv_FloatToText(NewManaRegenRate, ERoundingMode::HalfToEven, false, true, 1, 4, 1, 1);
//    FFormatNamedArguments Args;
//    Args.Add("Regen", ManaRegenRateText);
//    FText OutputText = FText::Format(FTextFormat::FromString(TEXT("+ {Regen}")), Args);
//    TextBlock_ManaRegen->SetText(OutputText);
//}
//
//void UGASHUDWidget::ShowAbilityConfirmCancelText(bool ShowText) 
//{
//    TextBlock_ConfirmCancel->SetVisibility(ShowText ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
//}
//
//void UGASHUDWidget::SetExperience(int32 NewExperience) 
//{
//    FText ExperienceText = UKismetTextLibrary::Conv_IntToText(NewExperience, false, true, 1, 4);
//    TextBlock_XP->SetText(ExperienceText);
//}
//
//void UGASHUDWidget::SetHeroLevel(int32 NewHeroLevel) 
//{
//    // Not implemented
//}
//
//void UGASHUDWidget::SetGold(int32 NewGold) 
//{
//    FText GoldText = UKismetTextLibrary::Conv_IntToText(NewGold, false, true, 1, 4);
//    TextBlock_Gold->SetText(GoldText);
//}
//
//void UGASHUDWidget::SetMeteorCountdown(float TimeRemaining) 
//{
//    if (TimeRemaining < 0.f)
//    {
//        // Local predicted cooldown, just gray out the image
//        Image_Meteor->SetOpacity(0.2f);
//        return;
//    }
//
//    // Server's cooldown -> start the timer
//    Image_Meteor->SetOpacity(0.2f);
//    TextBlock_CooldownTimeMeteor->SetVisibility(ESlateVisibility::Visible);
//
//    MeteorCooldownTimeRemaining = TimeRemaining;
//
//    // Setting the timer so that UpdateMeteorCountdown is called every 0.1 seconds (bLooping=true)
//    // GetWorldTimerManager().SetTimer(MeteorCooldownTimerHandle, this, &UGASHUDWidget::UpdateMeteorCountdown, 0.1f, true);
//    GetWorld()->GetTimerManager().SetTimer(MeteorCooldownTimerHandle, this, &UGASHUDWidget::UpdateMeteorCountdown, 0.1f, true);
//}
//
//void UGASHUDWidget::EndMeteorCountdown() 
//{
//    // The cooldown GE has ended, show the meteor again and hide the timer textblock
//    Image_Meteor->SetOpacity(1.f);
//    TextBlock_CooldownTimeMeteor->SetVisibility(ESlateVisibility::Collapsed);
//}
//
//void UGASHUDWidget::UpdateMeteorCountdown() 
//{
//    FText CooldownTimeRemainingText = UKismetTextLibrary::Conv_FloatToText(MeteorCooldownTimeRemaining, ERoundingMode::HalfToEven, false, true, 1, 2, 1, 1);
//    TextBlock_CooldownTimeMeteor->SetText(CooldownTimeRemainingText);
//    MeteorCooldownTimeRemaining -= 0.1f;
//    if (MeteorCooldownTimeRemaining <= 0.f && MeteorCooldownTimerHandle.IsValid())
//    {
//        GetWorld()->GetTimerManager().ClearTimer(MeteorCooldownTimerHandle);
//        TextBlock_CooldownTimeMeteor->SetVisibility(ESlateVisibility::Collapsed);
//    }
//    
//}
//
//void UGASHUDWidget::UpdateArmorStacks(int32 NewStackCount) 
//{
//    FText NumOfStacksText = UKismetTextLibrary::Conv_IntToText(NewStackCount, false, true, 1, 1);
//    TextBlock_ArmorStacks->SetText(NumOfStacksText);
//}
//
//void UGASHUDWidget::SetHealth() 
//{
//    FText CurrentHealthText = UKismetTextLibrary::Conv_FloatToText(CurrentHealth, ERoundingMode::HalfToEven, false, true, 1, 4, 0, 0);
//    FText MaxHealthText = UKismetTextLibrary::Conv_FloatToText(MaxHealth, ERoundingMode::HalfToEven, false, true, 1, 4, 0, 0);
//    FFormatNamedArguments Args;
//    Args.Add("Current", CurrentHealthText);
//    Args.Add("Max", MaxHealthText);
//    FText OutputText = FText::Format(FTextFormat::FromString(TEXT("{Current} / {Max}")), Args);
//    TextBlock_Health->SetText(OutputText);
//
//    SetHealthPercentage(CurrentHealth / (MaxHealth >= 1.f ? MaxHealth : 1.f));
//}
//
//void UGASHUDWidget::SetStamina() 
//{
//    FText CurrentStaminaText = UKismetTextLibrary::Conv_FloatToText(CurrentStamina, ERoundingMode::HalfToEven, false, true, 1, 4, 0, 0);
//    FText MaxStaminaText = UKismetTextLibrary::Conv_FloatToText(MaxStamina, ERoundingMode::HalfToEven, false, true, 1, 4, 0, 0);
//    FFormatNamedArguments Args;
//    Args.Add("Current", CurrentStaminaText);
//    Args.Add("Max", MaxStaminaText);
//    FText OutputText = FText::Format(FTextFormat::FromString(TEXT("{Current} / {Max}")), Args);
//    TextBlock_Stamina->SetText(OutputText);
//
//    SetStaminaPercentage(CurrentStamina / (MaxStamina >= 1.f ? MaxStamina : 1.f));
//}
//
//void UGASHUDWidget::SetMana() 
//{
//    FText CurrentManaText = UKismetTextLibrary::Conv_FloatToText(CurrentMana, ERoundingMode::HalfToEven, false, true, 1, 4, 0, 0);
//    FText MaxManaText = UKismetTextLibrary::Conv_FloatToText(MaxMana, ERoundingMode::HalfToEven, false, true, 1, 4, 0, 0);
//    FFormatNamedArguments Args;
//    Args.Add("Current", CurrentManaText);
//    Args.Add("Max", MaxManaText);
//    FText OutputText = FText::Format(FTextFormat::FromString(TEXT("{Current} / {Max}")), Args);
//    TextBlock_Mana->SetText(OutputText);
//
//    SetManaPercentage(CurrentMana / (MaxMana >= 1.f ? MaxMana : 1.f));
//}

void UHUDWidget::NativeConstruct() {
	Super::NativeConstruct();
}
