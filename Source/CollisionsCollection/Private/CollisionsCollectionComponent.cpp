// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionsCollectionComponent.h"

#include "CollisionsCollectionSubsystem.h"
#include "Components/SphereComponent.h"

// https://stackoverflow.com/questions/14421656/is-there-widely-available-wide-character-variant-of-file
#define WIDE2(X) L##X
#define WIDE1(X) WIDE2(X)
#define WFUNCTION WIDE1(__FUNCTION__)

UCollisionsCollectionComponent::UCollisionsCollectionComponent()
    : _collectionTag("")
    , _collectionCollisionProfileName("")
    , _generateOverlapEvents(true)
    , _collectionSpheres()
    , _collectionElements()
    , _externalsToOverlappingElements() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UCollisionsCollectionComponent::UpdateOverlaps() {
    for (auto element : _collectionElements) {
        element->UpdateOverlaps();
    }
}

void UCollisionsCollectionComponent::GetOverlappingComponents(TSet<UPrimitiveComponent*>& componentsOverlappingCollection) const {
    componentsOverlappingCollection.Reset();

    for (auto element : _collectionElements) {
        auto componentsOverlappingElement = TSet<UPrimitiveComponent*>();
        element->GetOverlappingComponents(componentsOverlappingElement);

        componentsOverlappingCollection.Append(componentsOverlappingElement);
    }

    // Collection elements must ignore each other, they must look like a single collision for client classes. UPrimitiveComponent::GetOverlappingComponents(),
    // so we need to take care of removing collection elements from the out parameter.
    for (auto element : _collectionElements) {
        [[maybe_unused]] const int32 nRemoved = componentsOverlappingCollection.Remove(element);
        check(nRemoved == 0 || nRemoved == 1);
    }
}

bool UCollisionsCollectionComponent::HasElement(const UPrimitiveComponent* collectionElement) const {
    return _collectionElements.Find(collectionElement) != nullptr;
}

const TArray<FCollectionSphereParameters>& UCollisionsCollectionComponent::GetCollectionSpheres() const {
    return _collectionSpheres;
}

void UCollisionsCollectionComponent::EndPlay(const EEndPlayReason::Type endPlayReason) {
    Super::EndPlay(endPlayReason);

    UCollisionsCollectionSubsystem* collectionsSubsys = GetWorld()->GetSubsystem<UCollisionsCollectionSubsystem>();
    collectionsSubsys->RemoveCollection(this);
}

void UCollisionsCollectionComponent::BeginPlay() {
    Super::BeginPlay();

    _collectSpheres();

    UCollisionsCollectionSubsystem* collectionsSubsys = GetWorld()->GetSubsystem<UCollisionsCollectionSubsystem>();
    collectionsSubsys->AddCollection(this);

    OnCollectionPlayBegun.Broadcast();
}

void UCollisionsCollectionComponent::_collectSpheres() {
    const TArray<UActorComponent*> sphereActors = GetOwner()->GetComponentsByTag(USphereComponent::StaticClass(), _collectionTag);
    _collectionSpheres.Reserve(sphereActors.Num());
    _collectionElements.Reserve(sphereActors.Num());

    for (auto sphereActor : sphereActors) {
        const auto sphere = Cast<USphereComponent>(sphereActor);
        check(sphere);

        const auto collisionProfile = sphere->GetCollisionProfileName();
        if (collisionProfile != _collectionCollisionProfileName) {
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Collision tagged as %s has Profile %s instead of %s, replacing."), *_collectionTag.ToString(),
                *collisionProfile.ToString(), *_collectionCollisionProfileName.ToString());

            sphere->SetCollisionProfileName(_collectionCollisionProfileName);
        }

        const bool generateOverlapEvents = sphere->GetGenerateOverlapEvents();
        if (generateOverlapEvents != _generateOverlapEvents) {
            const auto collisionString = generateOverlapEvents ? TEXT("true") : TEXT("false");
            const auto collectionString = _generateOverlapEvents ? TEXT("true") : TEXT("false");
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Collision has \"generate overlap events\" set to %s instead of %s, replacing."), collisionString,
                collectionString);

            sphere->SetGenerateOverlapEvents(_generateOverlapEvents);
        }

        FCollectionSphereParameters sphereParameters;
        sphereParameters.RootRelativeTransform = sphere->GetRelativeTransform();
        sphereParameters.Radius = sphere->GetScaledSphereRadius();
        _collectionSpheres.Add(MoveTemp(sphereParameters));

        sphere->OnComponentBeginOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementBeginOverlap);
        sphere->OnComponentEndOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementEndOverlap);

        _collectionElements.Emplace(sphere);
    }
}

void UCollisionsCollectionComponent::_collectionElementBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
    UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
    // Collection elements must not fire overlap events with each other, they must look like a single collision for client classes.
    if (!_collectionElements.Find(otherComp)) {
        int* overlappingElements = nullptr;

        // Did the collection just start overlapping with otherComp?
        if (overlappingElements = _externalsToOverlappingElements.Find(otherComp); !overlappingElements) {
            overlappingElements = &_externalsToOverlappingElements.Emplace(otherComp, 0);
            OnCollectionBeginOverlap.Broadcast(overlappedComponent, otherActor, otherComp, otherBodyIndex, bFromSweep, sweepResult);
        }

        ++(*overlappingElements);
    }
}

void UCollisionsCollectionComponent::_collectionElementEndOverlap(
    UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
    // Collection elements must not fire overlap events with each other, they must look like a single collision for client classes.
    if (!_collectionElements.Find(otherComp)) {
        int& overlappingElements = _externalsToOverlappingElements.FindChecked(otherComp);
        check(overlappingElements != 0);

        // Decrease _overlappingElements. If counter is now 0 fire event saying collection is not overlapping anymore
        --overlappingElements;
        if (overlappingElements == 0) {
            OnCollectionEndOverlap.Broadcast(overlappedComponent, otherActor, otherComp, otherBodyIndex);
            _externalsToOverlappingElements.Remove(otherComp);
        }
    }
}
