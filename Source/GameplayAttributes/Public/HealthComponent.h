// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, double);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChanged, double);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthRegenChanged, double);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYATTRIBUTES_API UHealthComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UHealthComponent();

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	double GetCurrentHealth() const { return _currentHealth; }
	double GetMaxHealth() const { return _maxHealth; }
	double GetHealthRegen() const { return _healthRegenPerSecond; }

	void SetCurrentHealth(double health);
	void SetMaxHealth(double maxHealth, bool bUpdateHealth = true);
	void SetHealthRegen(double healthRegenPerSecond);

	void TakeDamage(double damage) { 
		SetCurrentHealth(GetCurrentHealth() - damage);
		UE_LOG(LogTemp, Warning, TEXT("Damage taken: %f, Health is now: %f"), damage, _currentHealth);
	}

	bool IsDead() const { return _currentHealth <= DBL_EPSILON; }

	FOnHealthChanged& OnHealthChanged() { return _onHealthChanged; }
	FOnMaxHealthChanged& OnMaxHealthChanged() { return _onMaxHealthChanged; }
	FOnHealthRegenChanged& OnHealthRegenChanged() { return _onHealthRegenChanged; }

private:
	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "0"))
	double _maxHealth = 0.;

	// How much health we regenerate in 1 second
	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "0"))
	double _healthRegenPerSecond = 0.;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	double _currentHealth = 0.;

	FOnHealthChanged _onHealthChanged;
	FOnMaxHealthChanged _onMaxHealthChanged;
	FOnHealthRegenChanged _onHealthRegenChanged;
};
