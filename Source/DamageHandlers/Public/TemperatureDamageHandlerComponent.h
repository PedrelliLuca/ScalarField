// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TemperatureDamageHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DAMAGEHANDLERS_API UTemperatureDamageHandlerComponent : public UActorComponent {
	GENERATED_BODY()

public:	
    UTemperatureDamageHandlerComponent();

    void HandleDamage(double temperature);

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

    void SetMinComfortTemperature(double minComfortTemperature);
    void SetMaxComfortTemperature(double maxComfortTemperature);

    double GetMinComfortTemperature() const { return _minComfortTemperature; }
    double GetMaxComfortTemperature() const { return _maxComfortTemperature; }

    bool IsTemperatureComfortable(const double temperature) const { return temperature >= _minComfortTemperature && temperature <= _maxComfortTemperature; }

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

private:
    // TODO: in the future this might become a strategy object so that designers can set the damage function they want!
    double _computeDamageFromTemperature(double temperature);

    UPROPERTY(EditAnywhere, Category = "Temperature Comfort Interval", meta = (ClampMin = "0"))
    double _minComfortTemperature = 0.;

    UPROPERTY(EditAnywhere, Category = "Temperature Comfort Interval", meta = (ClampMin = "0"))
    double _maxComfortTemperature = 0.;

    UPROPERTY(EditAnywhere, Category = "Temperature Damage Interval", meta = (ClampMin = "0"))
    double _damageInterval = 1.;

    double _currentInteralTime = 0.;
};
