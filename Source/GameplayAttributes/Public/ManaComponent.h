// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "ManaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnManaChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxManaChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnManaRegenChanged, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMEPLAYATTRIBUTES_API UManaComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UManaComponent();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    float GetCurrentMana() const { return _currentMana; }
    float GetMaxMana() const { return _maxMana; }
    float GetManaRegen() const { return _manaRegenPerSecond; }

    void SetCurrentMana(float mana);
    void SetMaxMana(float maxMana, bool bUpdateMana = true);
    void SetManaRegen(float manaRegenPerSecond);

    void RestoreMana(float manaToRestore) { SetCurrentMana(GetCurrentMana() + manaToRestore); }

    FOnManaChanged& OnManaChanged() { return _onManaChanged; }
    FOnMaxManaChanged& OnMaxManaChanged() { return _onMaxManaChanged; }
    FOnManaRegenChanged& OnManaRegenChanged() { return _onManaRegenChanged; }

protected:
    void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Mana", meta = (ClampMin = "0"))
    float _maxMana = 0.;

    // How much mana we regenerate in 1 second
    UPROPERTY(EditAnywhere, Category = "Mana", meta = (ClampMin = "0"))
    float _manaRegenPerSecond = 0.;

    UPROPERTY(VisibleAnywhere, Category = "Mana")
    float _currentMana = 0.;

    FOnManaChanged _onManaChanged;
    FOnMaxManaChanged _onMaxManaChanged;
    FOnManaRegenChanged _onManaRegenChanged;
};
