// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThermodynamicComponent.h"

#include "TemperatureDamageHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DAMAGEHANDLERS_API UTemperatureDamageHandlerComponent : public UActorComponent {
	GENERATED_BODY()

public:	
    UTemperatureDamageHandlerComponent();

    void SetThermodynamicDamageDealer(TWeakObjectPtr<UThermodynamicComponent> thermodynamicC) { 
        check(thermodynamicC.IsValid());
        check(GetOwner() == thermodynamicC->GetOwner());
        _thermodynamicC = MoveTemp(thermodynamicC);
    }

    void HandleDamage();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

    void SetMinComfortTemperature(double minComfortTemperature);
    void SetMaxComfortTemperature(double maxComfortTemperature);

    double GetMinComfortTemperature() const { return _minComfortTemperature; }
    double GetMaxComfortTemperature() const { return _maxComfortTemperature; }

    bool IsTemperatureComfortable() const { 
        check(_thermodynamicC.IsValid());
        return _thermodynamicC->GetTemperature() >= _minComfortTemperature && _thermodynamicC->GetTemperature() <= _maxComfortTemperature;
    }

#if WITH_EDITOR
	void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

private:
    // TODO: in the future this might become a strategy object so that designer can set the damage function they want!
    double _computeDamageFromTemperature(double temperature);

    UPROPERTY(EditAnywhere, Category = "Temperature Comfort Interval", meta = (ClampMin = "0"))
    double _minComfortTemperature = 0.;

    UPROPERTY(EditAnywhere, Category = "Temperature Comfort Interval", meta = (ClampMin = "0"))
    double _maxComfortTemperature = 0.;

    UPROPERTY(EditAnywhere, Category = "Temperature Damage Interval", meta = (ClampMin = "0"))
    double _damageInterval = 1.;

    TWeakObjectPtr<UThermodynamicComponent> _thermodynamicC = nullptr;

    double _currentInteralTime = 0.;
};
