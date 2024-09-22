// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "PawnBindableWidgetInterface.h"

#include "HUDWidget.generated.h"

class UGameplayAttributesWidget;
class UHealthComponent;
class UNewStateComponent;
class USkillsContainerComponent;
class USkillIconContainerWidget;
class UThermometerWidget;

/**
 *
 */
UCLASS(Blueprintable)
class GAMEPLAYUI_API UHUDWidget : public UUserWidget, public IPawnBindableWidget {
    GENERATED_BODY()

public:
    void SetPawn(TWeakObjectPtr<APawn> pawn) override;
    void ForgetCurrentPawn() override;
    void Show() override;
    void Hide() override;

    void InitSkillIconContainer(TObjectPtr<USkillsContainerComponent> const& skillsContainer, TObjectPtr<UNewStateComponent> const& stateMachine);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void _setHealth();
    UFUNCTION(BlueprintImplementableEvent)
    void _setHealthRegen(float newHealthRegen);

    UFUNCTION(BlueprintImplementableEvent)
    void _setMana();
    UFUNCTION(BlueprintImplementableEvent)
    void _setManaRegen(double newManaRegen);

    UFUNCTION(BlueprintImplementableEvent)
    void _setTemperature(double temperature, FLinearColor temperatureColor);

    UFUNCTION(BlueprintImplementableEvent)
    void _setPauseStatus(bool bIsGamePaused);

private:
    void _bindAll();
    void _unbindAll();

    void _onDeath(TObjectPtr<AActor> _);

    void _onTacticalPauseToggle(bool bIsTacticalPauseOn, double);

    FDelegateHandle _onDeathHandle;
    FDelegateHandle _pauseToggleHandle;

    TWeakObjectPtr<UHealthComponent> _healthC = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USkillIconContainerWidget> _skillIconContainer = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UThermometerWidget> _thermometerWidget = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UGameplayAttributesWidget> _gameplayAttributesWidget = nullptr;
};
