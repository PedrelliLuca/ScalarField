// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermodynamicsInteractorComponent.h"

#include "Algo/ForEach.h"
#include "CollisionsCollectionComponent.h"
#include "GameFramework/Actor.h"
#include "HeatmapGridOperations.h"
#include "ThermodynamicsSubsystem.h"

// #define LOG_THERMODYNAMICS

UThermodynamicsInteractorComponent::UThermodynamicsInteractorComponent()
    : _initialTemperature(273.15f) // 0 Celsius degrees.
    , _heatCapacity(1.0f)
    , _currentTemperature(0.0f)
    , _nextTemperature(0.0f)
    , _hasNeverTicked(true)
    , _unregisteredDeltaTemperature(0.0f) {
    PrimaryComponentTick.bCanEverTick = true;

    // Why a delayed tick group on spawn?
    // We want _hasNeverTicked == true for the entirity of the first frame, so that it can't become false before all components that have this one in
    // _collidingInteractors have ticked, avoiding them increasing this component's counter and cause this to overshoot next frame.
    // To ensure this, we make new thermodynamic components tick as late as possible.
    PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostUpdateWork;
}

float UThermodynamicsInteractorComponent::GetTemperature() const {
    return _currentTemperature;
}

void UThermodynamicsInteractorComponent::SetTemperature(float const newTemperature) {
    _currentTemperature = newTemperature;
    _nextTemperature = newTemperature;
}

void UThermodynamicsInteractorComponent::TickComponent(float const deltaTime, ELevelTick const tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    // Suppose this just spawned and some of the _collidingInteractors have already _setCurrentTemperatureAsNext(). Updating their counters would cause them to
    // overshoot the next frame. This is why, on the frame we spawn, we skip thermodynamics.
    if (_hasNeverTicked) {
        _hasNeverTicked = false;                    // Ensures that, next frame, others won't overshoot because of this.
        SetTickGroup(ETickingGroup::TG_PrePhysics); // Ensures that, next frame, this won't overshoot because of others.

#ifdef LOG_THERMODYNAMICS
        auto thisName = FString(GetOwner()->GetName());
        UE_LOG(LogTemp, Warning, TEXT("%s will tick in pre-physics from now on"), *thisName);
#endif
        return;
    }

    // 1) Interact with other UThermodynamicsInteractorComponents you're colliding with
    _timesToBeCheckedThisFrame = _collidingInteractors.Num();
#ifdef LOG_THERMODYNAMICS
    auto thisName = FString(GetOwner()->GetName());
    UE_LOG(LogTemp, Warning, TEXT("%s will be checked %i times this frame"), *thisName, _timesToBeCheckedThisFrame);
#endif

    float const currDeltaT_OtherBodies = _interactWithOtherComponents(deltaTime);

    // 2) Interact with the Heatmap Grid
    check(_collisionsCollectionC.IsValid());
    FTransform const& interactorTransform = GetOwner()->GetTransform();
    float const currDeltaT_GridNormalized = HeatmapGrid::Interact(interactorTransform, _collisionsCollectionC.Get(), _currentTemperature, deltaTime);

    float const totalCurrDeltaT = currDeltaT_GridNormalized + currDeltaT_OtherBodies + _unregisteredDeltaTemperature;
    float const totalDeltaT = (UThermodynamicsSubsystem::ROD_CONSTANT * totalCurrDeltaT * deltaTime) / _heatCapacity;

    _nextTemperature += totalDeltaT;
    _unregisteredDeltaTemperature = 0.0f;

    if (_counterOfChecksThisFrame == _timesToBeCheckedThisFrame) {
        _setCurrentTemperatureAsNext();
    } else {
        // If the _counter overshoots the limit thermodyanmics is going to cease for this actor, there is a flaw in the spawn logic
        check(_counterOfChecksThisFrame < _timesToBeCheckedThisFrame);
    }

    check(_interactorsToRepay.IsEmpty());
}

#if WITH_EDITOR
void UThermodynamicsInteractorComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
    FProperty* const property = propertyChangedEvent.Property;
    FName const propertyName = property != nullptr ? property->GetFName() : NAME_None;
    if (propertyName == GET_MEMBER_NAME_CHECKED(UThermodynamicsInteractorComponent, _initialTemperature)) {
        if (auto const initTempProperty = CastFieldChecked<FFloatProperty>(property)) {
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

#ifdef LOG_THERMODYNAMICS
    auto thisName = FString(GetOwner()->GetName());
    UE_LOG(LogTemp, Warning, TEXT("%s begins play"), *thisName);
#endif

    SetTemperature(_initialTemperature);
    _retrieveThermodynamicCollision();
}

float UThermodynamicsInteractorComponent::_interactWithOtherComponents(float const deltaTime) {
    float deltaTemperature = 0.0f;

    for (TWeakObjectPtr<UThermodynamicsInteractorComponent> const& otherThermoIntC : _collidingInteractors) {
        if (!otherThermoIntC->_hasNeverTicked) {
            // This currDeltaT is from this component'sPOV, which is why its _currentTemperature is on the right hand side.
            // If this interactor's T is greater than the other's, deltaT < 0 => this interactor emits heat (deltaTemperature decreases).
            // If this interactor's T is smaller than the other's, deltaT > 0 => this interactor absorbs heat (deltaTemperature increases).
            deltaTemperature += otherThermoIntC->_currentTemperature - _currentTemperature;

            if (_interactorsToRepay.Contains(otherThermoIntC)) {
                // Our debt with otherThermoIntC this frame has been repaid
                _interactorsToRepay.Remove(otherThermoIntC);
            } else {
                // otherThermoC owes us an interaction this frame. It will have to repay during its tick or while unregistering us.
                otherThermoIntC->_interactorsToRepay.Emplace(this);
            }

            // We heat-checked otherThermoIntC, so it must increase its counter
            otherThermoIntC->_updateNumberOfInteractionsThisFrame();
        } else {
            // otherThermoIntC cannot interact with us this frame, but it's already in _collidingInteractors => it takes part in _timesToBeCheckedThisFrame, so
            // we need to pretend it interacted with us.
            _updateNumberOfInteractionsThisFrame();
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
#ifdef LOG_THERMODYNAMICS
    auto thisName = FString(GetOwner()->GetName());
    UE_LOG(LogTemp, Warning, TEXT("%s interacted %i times this frame"), *thisName, _counterOfChecksThisFrame);
#endif

    if (_nextTemperature != _currentTemperature) {
        _currentTemperature = _nextTemperature;
        OnTemperatureChanged.Broadcast(_currentTemperature);
    }

    _counterOfChecksThisFrame = 0;
    _timesToBeCheckedThisFrame = TNumericLimits<uint32>::Max();
}

void UThermodynamicsInteractorComponent::_retrieveThermodynamicCollision() {
    UCollisionsCollectionComponent* const collisionsCollectionC = GetOwner()->FindComponentByClass<UCollisionsCollectionComponent>();
    if (IsValid(collisionsCollectionC)) {
        _collisionsCollectionC = collisionsCollectionC;

        // Setup the simultaneity mechanism.
        _counterOfChecksThisFrame = 0;
        _timesToBeCheckedThisFrame = TNumericLimits<uint32>::Max();
        _collidingInteractors.Empty();

        // When a UCollisionsCollectionComponent spawns, even if it's already colliding with something, OnCollectionBeginOverlap doesn't Broadcast().
        // _setInitialInteractors detects if some other AActor is already interacting with us thermodynamically. In case the collection begins play after us,
        // schedule it for later.
        if (_collisionsCollectionC->HasBegunPlay()) {
            _setInitialInteractors();
        } else {
            _collisionsCollectionC->OnCollectionPlayBegun.AddUObject(this, &UThermodynamicsInteractorComponent::_setInitialInteractors);
        }

        // Setup of the interactors' registration on overlap status change.
        _collisionsCollectionC->OnCollectionBeginOverlap.AddUObject(this, &UThermodynamicsInteractorComponent::_registerInteractor);
        _collisionsCollectionC->OnCollectionEndOverlap.AddUObject(this, &UThermodynamicsInteractorComponent::_unregisterInteractor);
    } else {
        UE_LOG(LogTemp, Error, TEXT("UCollisionsCollectionComponent not found. No thermodynamics for this actor!"));
        SetComponentTickEnabled(false);
    }
}

void UThermodynamicsInteractorComponent::_setInitialInteractors() {
    // Do not call this function if _collisionsCollectionC hasn't been set.
    check(_collisionsCollectionC.IsValid());

    // Overlaps are messed up without this call.
    // I think this might be due to this component ticking on TG_PostUpdateWork the 1st frame.
    _collisionsCollectionC->UpdateOverlaps();

    TSet<UPrimitiveComponent*> overlappingCollisions;
    _collisionsCollectionC->GetOverlappingComponents(overlappingCollisions);

#ifdef LOG_THERMODYNAMICS
    auto thisName = FString(GetOwner()->GetName());
    UE_LOG(LogTemp, Warning, TEXT("%s initial interactors:"), *thisName);
#endif

    Algo::ForEach(overlappingCollisions, [&heatExchangers = _collidingInteractors](TObjectPtr<UPrimitiveComponent const> const otherCollision) {
        auto const otherOwner = otherCollision->GetOwner();
        auto const otherThermoIntC = otherOwner->FindComponentByClass<UThermodynamicsInteractorComponent>();
        // If I have a thermodynamic collision I must have a thermodynamic component
        check(IsValid(otherThermoIntC));
        heatExchangers.Emplace(otherThermoIntC);

#ifdef LOG_THERMODYNAMICS
        auto otherName = FString(otherOwner->GetName());
        UE_LOG(LogTemp, Warning, TEXT("%s"), *otherName);
#endif
    });
}

void UThermodynamicsInteractorComponent::_registerInteractor(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp,
    int32 otherBodyIndex, bool bFromSweep, FHitResult const& sweepResult) {
    auto const otherThermoIntC = otherActor->FindComponentByClass<UThermodynamicsInteractorComponent>();
    check(IsValid(otherThermoIntC));

    _collidingInteractors.Emplace(otherThermoIntC);

#ifdef LOG_THERMODYNAMICS
    auto thisName = FString(GetOwner()->GetName());
    auto otherName = FString(otherThermoIntC->GetOwner()->GetName());
    UE_LOG(LogTemp, Warning, TEXT("%s has a new interactor: %s"), *thisName, *otherName);
#endif
}

void UThermodynamicsInteractorComponent::_unregisterInteractor(
    UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
    TWeakObjectPtr<UThermodynamicsInteractorComponent>* found =
        Algo::FindByPredicate(_collidingInteractors, [otherComp](TWeakObjectPtr<UThermodynamicsInteractorComponent> const& thermoIntC) {
            return thermoIntC->_collisionsCollectionC->HasElement(otherComp);
        });

    // If this is nullptr, either:
    // 1. otherComp's UThermodynamicsInteractorC was never registered => there is a problem in the registration logic.
    // OR
    // 2. otherComp is not a THERMODYNAMICS_COLLISION_PROFILE_NAME => there is a problem on the collision channels.
    check(found != nullptr);

    auto thermoIntToRemove = *found;

    // During this frame, did thermoIntToRemove interact with us without us interacting with it? If so, we need to repay him right now, because we won't
    // see it anymore during tick!
    // This can happen is because, while UThermodynamicsInteractorComponents are ticking, AActors are still moving.
    if (_interactorsToRepay.Find(thermoIntToRemove)) {
        _interactorsToRepay.Remove(thermoIntToRemove);

        // _timesToBeCheckedThisFrame is evaluated at tick and thermoIntToRemove increased the counter before, so we must decrease it to avoid overshooting.
        --_counterOfChecksThisFrame;
        check(_counterOfChecksThisFrame >= 0);

        _unregisteredDeltaTemperature += thermoIntToRemove->_currentTemperature - _currentTemperature;
        thermoIntToRemove->_updateNumberOfInteractionsThisFrame();
    }

#ifdef LOG_THERMODYNAMICS
    auto thisName = FString(GetOwner()->GetName());
    auto otherName = FString(thermoIntToRemove->GetOwner()->GetName());
    UE_LOG(LogTemp, Warning, TEXT("%s lost an interactor: %s"), *thisName, *otherName);
#endif

    _collidingInteractors.Remove(thermoIntToRemove);
}
