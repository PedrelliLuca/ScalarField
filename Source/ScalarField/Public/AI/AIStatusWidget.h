// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "PawnBindableWidgetInterface.h"

#include "AIStatusWidget.generated.h"

class UHealthComponent;
class UManaComponent;
class UThermodynamicsInteractorComponent;

/**
 *
 */
UCLASS()
class SCALARFIELD_API UAIStatusWidget : public UUserWidget, public IPawnBindableWidget {
    GENERATED_BODY()

public:
    void SetPawn(TWeakObjectPtr<APawn> pawn) override;
    void ForgetCurrentPawn() override;
    void BindCurrentPawn() override;
    void UnbindCurrentPawn() override;
    void Show() override;
    void Hide() override;

    void SetComfortIntervalLimits(float upperLimit, float lowerLimit);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void _updateThermometer(float newTemperature);

    UFUNCTION(BlueprintImplementableEvent)
    void _updateHealth();

    UFUNCTION(BlueprintImplementableEvent)
    void _updateMana();

    void _setCurrentTemperature(float newTemperature);

    void _setCurrentHealth(float newHealth);
    void _setMaxHealth(float newMaxHealth);
    void _onDeath(TObjectPtr<AActor> _);

    void _setCurrentMana(double newMana);
    void _setMaxMana(double newMaxMana);

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

    FDelegateHandle _healthChangedHandle;
    FDelegateHandle _maxHealthChangedHandle;
    FDelegateHandle _onDeathHandle;
    FDelegateHandle _manaChangedHandle;
    FDelegateHandle _maxManaChangedHandle;
    FDelegateHandle _temperatureChangedHandle;
    FDelegateHandle _pauseToggleHandle;

    TWeakObjectPtr<UHealthComponent> _healthC = nullptr;
    TWeakObjectPtr<UManaComponent> _manaC = nullptr;
    TWeakObjectPtr<UThermodynamicsInteractorComponent> _thermoIntC = nullptr;
};
