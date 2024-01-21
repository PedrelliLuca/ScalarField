// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ThermodynamicsInteractorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTemperatureChanged, float);

/**
 * \brief TODO
 */
UCLASS(ClassGroup = (Thermodynamics), meta = (BlueprintSpawnableComponent))
class THERMODYNAMICSCORE_API UThermodynamicsInteractorComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UThermodynamicsInteractorComponent();

    float GetTemperature() const;

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

    FOnTemperatureChanged OnTemperatureChanged;

protected:
    void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere, Category = "Thermodynamic Parameters")
    float _initialTemperature;
    UPROPERTY(EditAnywhere, Category = "Thermodynamic Parameters")
    float _heatCapacity;

    UPROPERTY(EditAnywhere, Category = "Interaction Parameters")
    float _interactionRange;

    UPROPERTY(VisibleAnywhere, Category = "Thermodynamic Parameters")
    float _currentTemperature;
    float _nextTemperature;
};
