// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionsCollectionComponent.h"

#include "CollisionsCollectionSubsystem.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
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
    , _collectionBoxes()
    , _collectionElements()
    , _overlapsWithOtherCollectionElements()
    , _subsystem() {
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
        [[maybe_unused]] const int32 nRemoved = componentsOverlappingCollection.Remove(element.Get());
        check(nRemoved == 0 || nRemoved == 1);
    }
}

bool UCollisionsCollectionComponent::HasElement(const UPrimitiveComponent* collectionElement) const {
    return _collectionElements.Find(collectionElement) != nullptr;
}

const TArray<FCollectionSphereParameters>& UCollisionsCollectionComponent::GetCollectionSpheres() const {
    return _collectionSpheres;
}

const TArray<FCollectionBoxParameters>& UCollisionsCollectionComponent::GetCollectionBoxes() const {
    return _collectionBoxes;
}

const TArray<FCollectionCapsuleParameters>& UCollisionsCollectionComponent::GetCollectionCapsules() const {
    return _collectionCapsules;
}

void UCollisionsCollectionComponent::BeginDestroy() {
    Super::BeginDestroy();

    // We must make sure to not crash on these scenarios:
    // At the death of the World instance, the subsystem might have been collected already.
    // Moreover, if you change map from the editor, BeginDestroy() is called for all instances on the current map.
    if (_subsystem.IsValid()) {
        _subsystem->RemovePendingKillCollection(this);
    }
}

void UCollisionsCollectionComponent::BeginPlay() {
    Super::BeginPlay();

    _collectSpheres();
    _collectBoxes();
    _collectCapsules();

    _subsystem = GetWorld()->GetSubsystem<UCollisionsCollectionSubsystem>();
    _subsystem->AddCollection(this);

    OnCollectionPlayBegun.Broadcast();
}

void UCollisionsCollectionComponent::_collectSpheres() {
    const TArray<UActorComponent*> sphereActors = GetOwner()->GetComponentsByTag(USphereComponent::StaticClass(), _collectionTag);
    _collectionSpheres.Reserve(sphereActors.Num());

    for (const auto sphereActor : sphereActors) {
        const auto sphere = Cast<USphereComponent>(sphereActor);
        check(sphere);

        const auto collisionProfile = sphere->GetCollisionProfileName();
        if (collisionProfile != _collectionCollisionProfileName) {
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Sphere collision tagged as %s has Profile %s instead of %s, replacing."), *_collectionTag.ToString(),
                *collisionProfile.ToString(), *_collectionCollisionProfileName.ToString());

            sphere->SetCollisionProfileName(_collectionCollisionProfileName);
        }

        const bool generateOverlapEvents = sphere->GetGenerateOverlapEvents();
        if (generateOverlapEvents != _generateOverlapEvents) {
            const auto collisionString = generateOverlapEvents ? TEXT("true") : TEXT("false");
            const auto collectionString = _generateOverlapEvents ? TEXT("true") : TEXT("false");
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Sphere collision has \"generate overlap events\" set to %s instead of %s, replacing."), collisionString,
                collectionString);

            sphere->SetGenerateOverlapEvents(_generateOverlapEvents);
        }

        auto sphereParameters = FCollectionSphereParameters();
        sphereParameters.RootRelativeTransform = sphere->GetRelativeTransform();
        sphereParameters.Radius = sphere->GetScaledSphereRadius();
        _collectionSpheres.Add(MoveTemp(sphereParameters));

        sphere->OnComponentBeginOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementBeginOverlap);
        sphere->OnComponentEndOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementEndOverlap);

        _collectionElements.Emplace(sphere);
    }
}

void UCollisionsCollectionComponent::_collectBoxes() {
    const TArray<UActorComponent*> boxActors = GetOwner()->GetComponentsByTag(UBoxComponent::StaticClass(), _collectionTag);
    _collectionBoxes.Reserve(boxActors.Num());

    for (const auto boxActor : boxActors) {
        const auto box = Cast<UBoxComponent>(boxActor);
        check(box);

        const auto collisionProfile = box->GetCollisionProfileName();
        if (collisionProfile != _collectionCollisionProfileName) {
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Box collision tagged as %s has Profile %s instead of %s, replacing."), *_collectionTag.ToString(),
                *collisionProfile.ToString(), *_collectionCollisionProfileName.ToString());

            box->SetCollisionProfileName(_collectionCollisionProfileName);
        }

        const bool generateOverlapEvents = box->GetGenerateOverlapEvents();
        if (generateOverlapEvents != _generateOverlapEvents) {
            const auto collisionString = generateOverlapEvents ? TEXT("true") : TEXT("false");
            const auto collectionString = _generateOverlapEvents ? TEXT("true") : TEXT("false");
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Box collision has \"generate overlap events\" set to %s instead of %s, replacing."), collisionString,
                collectionString);

            box->SetGenerateOverlapEvents(_generateOverlapEvents);
        }

        auto boxParameters = FCollectionBoxParameters();
        boxParameters.RootRelativeTransform = box->GetRelativeTransform();
        const FVector boxExtent = box->GetScaledBoxExtent();
        boxParameters.BottomLeft = FVector(-boxExtent.X, -boxExtent.Y, 0.0);
        boxParameters.BottomRight = FVector(-boxExtent.X, boxExtent.Y, 0.0);
        boxParameters.TopRight = FVector(boxExtent.X, boxExtent.Y, 0.0);
        boxParameters.TopLeft = FVector(boxExtent.X, -boxExtent.Y, 0.0);
        _collectionBoxes.Add(MoveTemp(boxParameters));

        box->OnComponentBeginOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementBeginOverlap);
        box->OnComponentEndOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementEndOverlap);

        _collectionElements.Emplace(box);
    }
}

void UCollisionsCollectionComponent::_collectCapsules() {
    const TArray<UActorComponent*> capsuleActors = GetOwner()->GetComponentsByTag(UCapsuleComponent::StaticClass(), _collectionTag);
    _collectionBoxes.Reserve(capsuleActors.Num());

    for (const auto capsuleActor : capsuleActors) {
        const auto capsule = Cast<UCapsuleComponent>(capsuleActor);
        check(capsule);

        const auto collisionProfile = capsule->GetCollisionProfileName();
        if (collisionProfile != _collectionCollisionProfileName) {
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Capsule collision tagged as %s has Profile %s instead of %s, replacing."), *_collectionTag.ToString(),
                *collisionProfile.ToString(), *_collectionCollisionProfileName.ToString());

            capsule->SetCollisionProfileName(_collectionCollisionProfileName);
        }

        const bool generateOverlapEvents = capsule->GetGenerateOverlapEvents();
        if (generateOverlapEvents != _generateOverlapEvents) {
            const auto collisionString = generateOverlapEvents ? TEXT("true") : TEXT("false");
            const auto collectionString = _generateOverlapEvents ? TEXT("true") : TEXT("false");
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Capsule collision has \"generate overlap events\" set to %s instead of %s, replacing."), collisionString,
                collectionString);

            capsule->SetGenerateOverlapEvents(_generateOverlapEvents);
        }

        const float halfHeight = capsule->GetScaledCapsuleHalfHeight();
        const float radius = capsule->GetScaledCapsuleRadius();

        auto capsuleParameters = FCollectionCapsuleParameters();
        FCollectionBoxParameters& box = capsuleParameters.CylinderBox;
        box.RootRelativeTransform = capsule->GetRelativeTransform();
        box.BottomLeft = FVector(-radius, -radius, 0.0);
        box.BottomRight = FVector(-radius, radius, 0.0);
        box.TopRight = FVector(radius, radius, 0.0);
        box.TopLeft = FVector(radius, -radius, 0.0);

        FCollectionSphereParameters& upperSphere = capsuleParameters.UpperSphere;
        upperSphere.RootRelativeTransform = FTransform(halfHeight * FVector::UpVector) * box.RootRelativeTransform;
        upperSphere.Radius = radius;

        FCollectionSphereParameters& lowerSphere = capsuleParameters.LowerSphere;
        lowerSphere.RootRelativeTransform = FTransform(halfHeight * FVector::DownVector) * box.RootRelativeTransform;
        lowerSphere.Radius = radius;

        _collectionCapsules.Add(MoveTemp(capsuleParameters));

        capsule->OnComponentBeginOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementBeginOverlap);
        capsule->OnComponentEndOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementEndOverlap);

        _collectionElements.Emplace(capsule);
    }
}

void UCollisionsCollectionComponent::_collectionElementBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
    UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
    // Collection elements must not fire overlap events with each other, they must look like a single collision for client classes.
    if (!_collectionElements.Find(otherComp)) {
        TMap<TWeakObjectPtr<UPrimitiveComponent>, int>* elementsToOverlaps = nullptr;
        int* overlappingElements = nullptr;

        auto otherCollection = _subsystem->GetCollectionByElement(otherComp);
        if (elementsToOverlaps = _overlapsWithOtherCollectionElements.Find(otherCollection); !elementsToOverlaps) {
            // We were not already overlapping with the otherCollection, it's the first collision...
            elementsToOverlaps = &_overlapsWithOtherCollectionElements.Emplace(otherCollection, TMap<TWeakObjectPtr<UPrimitiveComponent>, int>());
            overlappingElements = &elementsToOverlaps->Emplace(otherComp, 0);

            // ...so we broadcast.
            OnCollectionBeginOverlap.Broadcast(overlappedComponent, otherActor, otherComp, otherBodyIndex, bFromSweep, sweepResult);
        } else if (overlappingElements = elementsToOverlaps->Find(otherComp); !overlappingElements) {
            // We are already overlapping with otherCollection, but not with its otherComp element
            overlappingElements = &elementsToOverlaps->Emplace(otherComp, 0);
        }

        ++(*overlappingElements);
    }
}

void UCollisionsCollectionComponent::_collectionElementEndOverlap(
    UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
    // Collection elements must not fire overlap events with each other, they must look like a single collision for client classes.
    if (!_collectionElements.Find(otherComp)) {
        auto otherCollection = _subsystem->GetCollectionByElement(otherComp);

        TMap<TWeakObjectPtr<UPrimitiveComponent>, int>& elementsToOverlaps = _overlapsWithOtherCollectionElements.FindChecked(otherCollection);
        check(!elementsToOverlaps.IsEmpty());
        int& overlappingElements = elementsToOverlaps.FindChecked(otherComp);
        check(overlappingElements != 0);

        // Decrease _overlappingElements. If counter is now 0...
        --overlappingElements;
        if (overlappingElements == 0) {
            [[maybe_unused]] int32 nRemoved = elementsToOverlaps.Remove(otherComp);
            check(nRemoved == 1);

            // ... check if we're still colliding with other elements of the collection. If not...
            if (elementsToOverlaps.IsEmpty()) {
                // ... fire event saying collection is not overlapping anymore.
                OnCollectionEndOverlap.Broadcast(overlappedComponent, otherActor, otherComp, otherBodyIndex);

                nRemoved = _overlapsWithOtherCollectionElements.Remove(otherCollection);
                check(nRemoved == 1);
            }
        }
    }
}
