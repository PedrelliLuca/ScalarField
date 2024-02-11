// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "TemperatureDamageHandlerComponent.generated.h"

class UHealthComponent;
class UThermodynamicsInteractorComponent;

UCLASS(ClassGroup = (DamageHandlers), meta = (BlueprintSpawnableComponent))
class DAMAGEHANDLERS_API UTemperatureDamageHandlerComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UTemperatureDamageHandlerComponent();

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

    void SetMinComfortTemperature(double minComfortTemperature);
    void SetMaxComfortTemperature(double maxComfortTemperature);

    double GetMinComfortTemperature() const { return _minComfortTemperature; }
    double GetMaxComfortTemperature() const { return _maxComfortTemperature; }

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

protected:
    void BeginPlay() override;

private:
    bool _isTemperatureCausingDamage() const;

    // TODO: in the future this might become a strategy object so that designers can set the damage function they want!
    float _computeDamageForTemperature(float temperature) const;

    UPROPERTY(EditAnywhere, Category = "Temperature Comfort Interval", meta = (ClampMin = "0"))
    double _minComfortTemperature = 0.;

    UPROPERTY(EditAnywhere, Category = "Temperature Comfort Interval", meta = (ClampMin = "0"))
    double _maxComfortTemperature = 0.;

    UPROPERTY(EditAnywhere, Category = "Temperature Damage Interval", meta = (ClampMin = "0"))
    double _damageInterval = 1.;

    FTimerHandle _damageCooldownTimerHandle;

    TWeakObjectPtr<UHealthComponent> _healthC;
    TWeakObjectPtr<UThermodynamicsInteractorComponent> _thermoInteractorC;
};
