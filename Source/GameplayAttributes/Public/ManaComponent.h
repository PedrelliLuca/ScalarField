// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ManaComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnManaChanged, double);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxManaChanged, double);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnManaRegenChanged, double);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPLAYATTRIBUTES_API UManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UManaComponent();

	void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	double GetCurrentMana() const { return _currentMana; }
	double GetMaxMana() const { return _maxMana; }
	double GetManaRegen() const { return _manaRegenPerSecond; }

	void SetCurrentMana(double mana);
	void SetMaxMana(double maxMana, bool bUpdateMana = true);
	void SetManaRegen(double manaRegenPerSecond);

	void RestoreMana(double manaToRestore) { SetCurrentMana(GetCurrentMana() + manaToRestore);}

	FOnManaChanged& OnManaChanged() { return _onManaChanged; }
	FOnMaxManaChanged& OnMaxManaChanged() { return _onMaxManaChanged; }
	FOnManaRegenChanged& OnManaRegenChanged() { return _onManaRegenChanged; }

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Mana", meta = (ClampMin = "0"))
	double _maxMana = 0.;

	// How much mana we regenerate in 1 second
	UPROPERTY(EditAnywhere, Category = "Mana", meta = (ClampMin = "0"))
	double _manaRegenPerSecond = 0.;

	UPROPERTY(VisibleAnywhere, Category = "Mana")
	double _currentMana = 0.;

	FOnManaChanged _onManaChanged;
	FOnMaxManaChanged _onMaxManaChanged;
	FOnManaRegenChanged _onManaRegenChanged;
};
