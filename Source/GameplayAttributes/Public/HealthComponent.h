// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthRegenChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeath, TObjectPtr<AActor>);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAMEPLAYATTRIBUTES_API UHealthComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UHealthComponent();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    float GetCurrentHealth() const { return _currentHealth; }
    float GetMaxHealth() const { return _maxHealth; }
    float GetHealthRegen() const { return _healthRegenPerSecond; }

    void SetCurrentHealth(float health);
    void SetMaxHealth(float maxHealth, bool bUpdateHealth = true);
    void SetHealthRegen(float healthRegenPerSecond);

    void TakeDamage(float damage) { SetCurrentHealth(GetCurrentHealth() - damage); }
    void Heal(float healthToAdd) { SetCurrentHealth(GetCurrentHealth() + healthToAdd); }

    UFUNCTION(BlueprintPure)
    bool IsDead() const { return _currentHealth <= DBL_EPSILON; }

    FOnHealthChanged& OnHealthChanged() { return _onHealthChanged; }
    FOnMaxHealthChanged& OnMaxHealthChanged() { return _onMaxHealthChanged; }
    FOnHealthRegenChanged& OnHealthRegenChanged() { return _onHealthRegenChanged; }
    FOnDeath& OnDeath() { return _onDeath; }

protected:
    void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "0"))
    float _maxHealth = 0.;

    // How much health we regenerate in 1 second
    UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "0"))
    float _healthRegenPerSecond = 0.;

    UPROPERTY(VisibleAnywhere, Category = "Health")
    float _currentHealth = 0.;

    FOnHealthChanged _onHealthChanged;
    FOnMaxHealthChanged _onMaxHealthChanged;
    FOnHealthRegenChanged _onHealthRegenChanged;
    FOnDeath _onDeath;
};
