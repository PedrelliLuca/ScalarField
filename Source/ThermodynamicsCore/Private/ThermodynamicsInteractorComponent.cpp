// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermodynamicsInteractorComponent.h"

#include "Algo/ForEach.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "HeatmapGridOperations.h"
#include "ThermodynamicsSubsystem.h"

UThermodynamicsInteractorComponent::UThermodynamicsInteractorComponent()
    : _initialTemperature(273.15f) // 0 Celsius degrees.
    , _heatCapacity(1.0f)
    , _currentTemperature(0.0f)
    , _nextTemperature(0.0f)
    , _hasNeverTicked(true) {
    PrimaryComponentTick.bCanEverTick = true;

    // Why a delayed tick group on spawn?
    // We want _hasNeverTicked == true for the entirity of the first frame, so that it can't become false before all components that have this one in
    // _heatExchangers have ticked, avoiding them increasing this component's counter and cause this to overshoot next frame.
    // To ensure this, we make new thermodynamic components tick as late as possible.
    PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostUpdateWork;
}

float UThermodynamicsInteractorComponent::GetTemperature() const {
    return _currentTemperature;
}

void UThermodynamicsInteractorComponent::SetTemperature(const float newTemperature) {
    _currentTemperature = newTemperature;
    _nextTemperature = newTemperature;
}

void UThermodynamicsInteractorComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    // Suppose this just spawned and some of the _heatExchangers have already _setCurrentTemperatureHasNext(). Updating their counters would cause them to
    // overshoot the next frame. This is why, on the frame we spawn, we skip thermodynamics.
    if (_hasNeverTicked) {
        _hasNeverTicked = false; // Ensures that, next frame, others won't overshoot because of this.
        SetTickGroup(ETickingGroup::TG_PrePhysics); // Ensures that, next frame, this won't overshoot because of others.
        return;
    }

    // 1) Interact with other UThermodynamicsInteractorComponents you're colliding with
    _timesToBeCheckedThisFrame = _heatExchangers.Num();
    const float currDeltaT_OtherBodies = _interactWithOtherComponents(deltaTime);

    // 2) Interact with the Heatmap Grid
    const auto location = FVector2D(GetOwner()->GetActorLocation());
    const float interactionRange = _sphereCollisionC->GetScaledSphereRadius();
    const float currDeltaT_GridNormalized = HeatmapGrid::Interact(location, interactionRange, _currentTemperature, deltaTime);

    const float totalCurrDeltaT = currDeltaT_GridNormalized + currDeltaT_OtherBodies;
    const float totalDeltaT = (UThermodynamicsSubsystem::ROD_CONSTANT * totalCurrDeltaT * deltaTime) / _heatCapacity;

    _nextTemperature += totalDeltaT;

    if (_counterOfChecksThisFrame == _timesToBeCheckedThisFrame) {
        _setCurrentTemperatureAsNext();
    } else {
        // If the _counter overshoots the limit thermodyanmics is going to cease for this actor, there is a flaw in the spawn logic
        check(_counterOfChecksThisFrame < _timesToBeCheckedThisFrame);
    }
}

#if WITH_EDITOR
void UThermodynamicsInteractorComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
    FProperty* const property = propertyChangedEvent.Property;
    const FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
    if (propertyName == GET_MEMBER_NAME_CHECKED(UThermodynamicsInteractorComponent, _initialTemperature)) {
        if (const auto initTempProperty = CastFieldChecked<FFloatProperty>(property)) {
            float initialTemperature = initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<float>(this));
            initialTemperature = FMath::Clamp(initialTemperature, 0., TNumericLimits<float>::Max());

            _initialTemperature = initialTemperature;
            _currentTemperature = initialTemperature;
            _nextTemperature = initialTemperature;
        }
    }

    Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UThermodynamicsInteractorComponent::BeginPlay() {
    Super::BeginPlay();

    SetTemperature(_initialTemperature);
    _retrieveThermodynamicCollision();
}

float UThermodynamicsInteractorComponent::_interactWithOtherComponents(const float deltaTime) {
    float deltaTemperature = 0.0f;

    for (const TWeakObjectPtr<UThermodynamicsInteractorComponent>& otherThermoIntC : _heatExchangers) {
        if (!otherThermoIntC->_hasNeverTicked) {
            // This currDeltaT is from this component'sPOV, which is why its _currentTemperature is on the right hand side.
            // If this interactor's T is greater than the other's, deltaT < 0 => this interactor emits heat (deltaTemperature decreases).
            // If this interactor's T is smaller than the other's, deltaT > 0 => this interactor absorbs heat (deltaTemperature increases).
            deltaTemperature += otherThermoIntC->_currentTemperature - _currentTemperature;

            // We heat-checked otherThermoIntC, so it must increase its counter
            otherThermoIntC->_updateNumberOfInteractionsThisFrame();
        }
    }

    return deltaTemperature;
}

void UThermodynamicsInteractorComponent::_updateNumberOfInteractionsThisFrame() {
    ++_counterOfChecksThisFrame;
    if (_counterOfChecksThisFrame == _timesToBeCheckedThisFrame) {
        _setCurrentTemperatureAsNext();
    }
}

void UThermodynamicsInteractorComponent::_setCurrentTemperatureAsNext() {
    if (_nextTemperature != _currentTemperature) {
        _currentTemperature = _nextTemperature;
        OnTemperatureChanged.Broadcast(_currentTemperature);
    }

    _counterOfChecksThisFrame = 0;
    _timesToBeCheckedThisFrame = TNumericLimits<uint32>::Max();
}

void UThermodynamicsInteractorComponent::_retrieveThermodynamicCollision() {
    bool successfullRetrieval = false;
    auto errorMessage = FString("");

    const TArray<UActorComponent*> possibleCollisions = GetOwner()->GetComponentsByTag(USphereComponent::StaticClass(), FName{THERMODYNAMICS_COLLISION_TAG});
    if (!possibleCollisions.IsEmpty()) {
        if (const auto sphereCollisionC = Cast<USphereComponent>(possibleCollisions[0]); IsValid(sphereCollisionC)) {
            const FName collisionProfileName = sphereCollisionC->GetCollisionProfileName();
            if (collisionProfileName == FName(THERMODYNAMICS_COLLISION_PROFILE_NAME)) {
                if (sphereCollisionC->GetGenerateOverlapEvents()) {
                    _sphereCollisionC = sphereCollisionC;
                    successfullRetrieval = true;

                    // Setup the simultaneity mechanism.
                    _counterOfChecksThisFrame = 0;
                    _timesToBeCheckedThisFrame = TNumericLimits<uint32>::Max();
                    _heatExchangers.Empty();

                    // When a UPrimitiveComponent spawns, even if it's already colliding with something, OnComponentBeginOverlap doesn't Broadcast().
                    // We need this to detect if some other AActor is already interacting with us thermodynamically.
                    _setInitialInteractors();

                    // Setup of the interactors' registration on overlap status change.
                    _sphereCollisionC->OnComponentBeginOverlap.AddDynamic(this, &UThermodynamicsInteractorComponent::_registerInteractor);
                    _sphereCollisionC->OnComponentEndOverlap.AddDynamic(this, &UThermodynamicsInteractorComponent::_unregisterInteractor);
                } else {
                    errorMessage = FString::Printf(TEXT("Collisions disabled on USphereComponent. No thermodynamics for this actor!"));
                }
            } else {
                errorMessage = FString::Printf(TEXT("Collision profile of USphereComponent is not %s. No thermodynamics for this actor!"),
                    *FString(THERMODYNAMICS_COLLISION_PROFILE_NAME));
            }
        } else {
            errorMessage =
                FString::Printf(TEXT("Tag %s found on a non-USphereComponent. No thermodynamics for this actor!"), *FString(THERMODYNAMICS_COLLISION_TAG));
        }
    } else {
        errorMessage = FString::Printf(TEXT("No component with tag %s was found. No thermodynamics for this actor!"), *FString(THERMODYNAMICS_COLLISION_TAG));
    }

    if (!successfullRetrieval) {
        UE_LOG(LogTemp, Error, TEXT("%s"), *errorMessage);
        SetComponentTickEnabled(false);
    }
}

void UThermodynamicsInteractorComponent::_setInitialInteractors() {
    // Do not call this function if _sphereCollisionC hasn't been set.
    check(_sphereCollisionC.IsValid());

    // Overlaps are messed up without this call.
    // I think this might be due to this component ticking on TG_PostUpdateWork the 1st frame.
    _sphereCollisionC->UpdateOverlaps();

    TArray<TObjectPtr<UPrimitiveComponent>> overlappingCollisions;
    _sphereCollisionC->GetOverlappingComponents(overlappingCollisions);

    Algo::ForEach(overlappingCollisions, [&heatExchangers = _heatExchangers](const TObjectPtr<const UPrimitiveComponent> otherCollision) {
        const auto otherOwner = otherCollision->GetOwner();
        const auto otherThermoIntC = otherOwner->FindComponentByClass<UThermodynamicsInteractorComponent>();
        // If I have a thermodynamic collision I must have a thermodynamic component
        check(IsValid(otherThermoIntC));
        heatExchangers.Emplace(otherThermoIntC);
    });
}

void UThermodynamicsInteractorComponent::_registerInteractor(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp,
    int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
    const auto otherThermoIntC = otherActor->FindComponentByClass<UThermodynamicsInteractorComponent>();
    check(IsValid(otherThermoIntC));

    _heatExchangers.Emplace(otherThermoIntC);
}

void UThermodynamicsInteractorComponent::_unregisterInteractor(
    UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
    TWeakObjectPtr<UThermodynamicsInteractorComponent>* thermoIntToRemove = Algo::FindByPredicate(_heatExchangers,
        [otherComp](const TWeakObjectPtr<UThermodynamicsInteractorComponent>& thermoIntC) { return thermoIntC->_sphereCollisionC == otherComp; });

    // If this is nullptr, either:
    // 1. otherComp is a UThermodynamicsInteractorC that was never registered => there is a problem in the registration logic.
    // OR
    // 2. otherComp is not a UThermodynamicsInteractorC => there is a problem on the collision channels.
    check(thermoIntToRemove != nullptr);
    _heatExchangers.Remove(*thermoIntToRemove);
}
