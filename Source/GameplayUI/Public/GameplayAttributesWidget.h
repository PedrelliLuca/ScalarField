// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "PawnBindableWidgetInterface.h"

#include "GameplayAttributesWidget.generated.h"

class UHealthComponent;
class UManaComponent;

/**
 *
 */
UCLASS(Blueprintable)
class GAMEPLAYUI_API UGameplayAttributesWidget : public UUserWidget, public IPawnBindableWidget {
    GENERATED_BODY()

public:
    void SetPawn(TWeakObjectPtr<APawn> pawn) override;
    void ForgetCurrentPawn() override;
    void Show() override;
    void Hide() override;

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void _setHealth();
    UFUNCTION(BlueprintImplementableEvent)
    void _setHealthRegen(float newHealthRegen);

    UFUNCTION(BlueprintImplementableEvent)
    void _setMana();
    UFUNCTION(BlueprintImplementableEvent)
    void _setManaRegen(double newManaRegen);

    UPROPERTY(BlueprintReadOnly, Category = "Health")
    float _currentHealth = 0.;
    UPROPERTY(BlueprintReadOnly, Category = "Health")
    float _maxHealth = 0.;

    UPROPERTY(BlueprintReadOnly, Category = "Mana")
    double _currentMana = 0.;
    UPROPERTY(BlueprintReadOnly, Category = "Mana")
    double _maxMana = 0.;

private:
    void _bindAll();
    void _unbindAll();

    void _setMaxHealth(float newMaxHealth);
    void _setCurrentHealth(float newCurrentHealth);
    void _setMaxMana(double newMaxMana);
    void _setCurrentMana(double newCurrentMana);

    FDelegateHandle _healthChangedHandle;
    FDelegateHandle _maxHealthChangedHandle;
    FDelegateHandle _healthRegenChangedHandle;
    FDelegateHandle _manaChangedHandle;
    FDelegateHandle _maxManaChangedHandle;
    FDelegateHandle _manaRegenChangedHandle;

    TWeakObjectPtr<UHealthComponent> _healthC = nullptr;
    TWeakObjectPtr<UManaComponent> _manaC = nullptr;
};
