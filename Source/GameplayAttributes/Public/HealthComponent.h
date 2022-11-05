// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYATTRIBUTES_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	double GetHealth() const { return _health; }
	void SetHealth(double health);
	void SetMaxHealth(double maxHealth, bool bUpdateHealth = true);

	void TakeDamage(double damage) { _health -= damage; }
	bool IsDead() const { return _health <= DBL_EPSILON; }

private:
	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "0"))
	double _maxHealth = 0.;

	// How much health we regenerate in 1 second
	UPROPERTY(EditAnywhere, Category = "Health", meta = (ClampMin = "0"))
	double _healthRegenPerSecond = 0.;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	double _health = 0.;
};
