// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ThermodynamicsInteractorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTemperatureChanged, float);

class UCollisionsCollectionComponent;

/**
 * \brief TODO
 */
UCLASS(ClassGroup = (Thermodynamics), meta = (BlueprintSpawnableComponent))
class THERMODYNAMICSCORE_API UThermodynamicsInteractorComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UThermodynamicsInteractorComponent();

    float GetTemperature() const;
    void SetTemperature(float newTemperature);

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

    FOnTemperatureChanged OnTemperatureChanged;

protected:
    void BeginPlay() override;

private:
    float _interactWithOtherComponents(float deltaTime);

    void _updateNumberOfInteractionsThisFrame();

    void _setCurrentTemperatureAsNext();

    void _retrieveThermodynamicCollision();

    void _setInitialInteractors();

    void _registerInteractor(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex,
        bool bFromSweep, const FHitResult& sweepResult);
    void _unregisterInteractor(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

    UPROPERTY(EditAnywhere, Category = "Thermodynamic Parameters")
    float _initialTemperature;
    UPROPERTY(EditAnywhere, Category = "Thermodynamic Parameters")
    float _heatCapacity;

    UPROPERTY(VisibleAnywhere, Category = "Thermodynamic Parameters")
    float _currentTemperature;
    float _nextTemperature;

    TWeakObjectPtr<UCollisionsCollectionComponent> _collisionsCollectionC;
    // The number of times this component will be checked this frame by other thermodynamic interactors
    uint32 _counterOfChecksThisFrame;
    // Count of how many times this component got checked by other thermodynamic component this frame. When the counter reaches _timesToBeCheckedThisFrame, we
    // know for sure we won't be checked again, meaning that we can finally set _currentTemperature = _nextTemperature
    uint32 _timesToBeCheckedThisFrame;

    // To make thermodynamics ignore this on the frame it spawns, avoiding 1 frame-off bugs. See explanation in ctor and TickComponent() functions.
    bool _hasNeverTicked;

    // Thermodynamic interactors whose collision is colliding with this component's. We're gonna exchange heat with them this frame.
    TSet<TWeakObjectPtr<UThermodynamicsInteractorComponent>> _collidingInteractors;

    // Thermodynamic interactors that interacted with us without us interacting with them yet.
    TSet<TWeakObjectPtr<UThermodynamicsInteractorComponent>> _interactorsToRepay;

    float _unregisteredDeltaTemperature;

    // TODO remove: this should be read via UDeveloperSettings
    /*inline static const char* THERMODYNAMICS_COLLISION_TAG = "ThermodynamicsCollision";
    inline static const char* THERMODYNAMICS_COLLISION_PROFILE_NAME = "HeatExchanger";*/
};
