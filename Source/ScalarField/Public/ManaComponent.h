// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCALARFIELD_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UManaComponent();

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	void SetMaxMana(double maxMana, bool bUpdateMana = true);
	void SetMana(double mana);
	double GetMana() const { return _mana; }

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY(EditAnywhere, Category = "Mana", meta = (ClampMin = "0"))
	double _maxMana = 0.;

	// How much mana we regenerate in 1 second
	UPROPERTY(EditAnywhere, Category = "Mana", meta = (ClampMin = "0"))
	double _manaRegenPerSecond = 0.;

	UPROPERTY(VisibleAnywhere, Category = "Mana")
	double _mana = 0.;
};
