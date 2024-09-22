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

namespace CollectionParameterHelpers {
void BoxSidesFiller(FCollectionBoxParameters& box, FVector const& boxExtent) {
    auto const v1 = FVector(-boxExtent.X, -boxExtent.Y, -boxExtent.Z);
    auto const v2 = FVector(-boxExtent.X, boxExtent.Y, -boxExtent.Z);
    auto const v3 = FVector(-boxExtent.X, boxExtent.Y, boxExtent.Z);
    auto const v4 = FVector(-boxExtent.X, -boxExtent.Y, boxExtent.Z);
    auto const v5 = FVector(boxExtent.X, -boxExtent.Y, -boxExtent.Z);
    auto const v6 = FVector(boxExtent.X, boxExtent.Y, -boxExtent.Z);
    auto const v7 = FVector(boxExtent.X, boxExtent.Y, boxExtent.Z);
    auto const v8 = FVector(boxExtent.X, -boxExtent.Y, boxExtent.Z);

    box.FrontFace[0] = v5;
    box.FrontFace[1] = v6;
    box.FrontFace[2] = v7;
    box.FrontFace[3] = v8;

    box.LeftFace[0] = v1;
    box.LeftFace[1] = v5;
    box.LeftFace[2] = v8;
    box.LeftFace[3] = v4;

    box.BackFace[0] = v2;
    box.BackFace[1] = v1;
    box.BackFace[2] = v4;
    box.BackFace[3] = v3;

    box.RightFace[0] = v6;
    box.RightFace[1] = v2;
    box.RightFace[2] = v3;
    box.RightFace[3] = v7;

    box.TopFace[0] = v8;
    box.TopFace[1] = v7;
    box.TopFace[2] = v3;
    box.TopFace[3] = v4;

    box.BottomFace[0] = v1;
    box.BottomFace[1] = v2;
    box.BottomFace[2] = v6;
    box.BottomFace[3] = v5;
}
} // namespace CollectionParameterHelpers

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
        [[maybe_unused]] int32 const nRemoved = componentsOverlappingCollection.Remove(element.Get());
        check(nRemoved == 0 || nRemoved == 1);
    }
}

bool UCollisionsCollectionComponent::HasElement(UPrimitiveComponent const* collectionElement) const {
    return _collectionElements.Find(collectionElement) != nullptr;
}

TArray<FCollectionSphereParameters> const& UCollisionsCollectionComponent::GetCollectionSpheres() const {
    return _collectionSpheres;
}

TArray<FCollectionBoxParameters> const& UCollisionsCollectionComponent::GetCollectionBoxes() const {
    return _collectionBoxes;
}

TArray<FCollectionCapsuleParameters> const& UCollisionsCollectionComponent::GetCollectionCapsules() const {
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
    TArray<UActorComponent*> const sphereActors = GetOwner()->GetComponentsByTag(USphereComponent::StaticClass(), _collectionTag);
    _collectionSpheres.Reserve(sphereActors.Num());

    for (auto const sphereActor : sphereActors) {
        auto const sphere = Cast<USphereComponent>(sphereActor);
        check(sphere);

        auto const collisionProfile = sphere->GetCollisionProfileName();
        if (collisionProfile != _collectionCollisionProfileName) {
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Sphere collision tagged as %s has Profile %s instead of %s, replacing."), *_collectionTag.ToString(),
                *collisionProfile.ToString(), *_collectionCollisionProfileName.ToString());

            sphere->SetCollisionProfileName(_collectionCollisionProfileName);
        }

        bool const generateOverlapEvents = sphere->GetGenerateOverlapEvents();
        if (generateOverlapEvents != _generateOverlapEvents) {
            auto const collisionString = generateOverlapEvents ? TEXT("true") : TEXT("false");
            auto const collectionString = _generateOverlapEvents ? TEXT("true") : TEXT("false");
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
    TArray<UActorComponent*> const boxActors = GetOwner()->GetComponentsByTag(UBoxComponent::StaticClass(), _collectionTag);
    _collectionBoxes.Reserve(boxActors.Num());

    for (auto const boxActor : boxActors) {
        auto const box = Cast<UBoxComponent>(boxActor);
        check(box);

        auto const collisionProfile = box->GetCollisionProfileName();
        if (collisionProfile != _collectionCollisionProfileName) {
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Box collision tagged as %s has Profile %s instead of %s, replacing."), *_collectionTag.ToString(),
                *collisionProfile.ToString(), *_collectionCollisionProfileName.ToString());

            box->SetCollisionProfileName(_collectionCollisionProfileName);
        }

        bool const generateOverlapEvents = box->GetGenerateOverlapEvents();
        if (generateOverlapEvents != _generateOverlapEvents) {
            auto const collisionString = generateOverlapEvents ? TEXT("true") : TEXT("false");
            auto const collectionString = _generateOverlapEvents ? TEXT("true") : TEXT("false");
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Box collision has \"generate overlap events\" set to %s instead of %s, replacing."), collisionString,
                collectionString);

            box->SetGenerateOverlapEvents(_generateOverlapEvents);
        }

        auto boxParameters = FCollectionBoxParameters();
        boxParameters.RootRelativeTransform = box->GetRelativeTransform();
        FVector const boxExtent = box->GetScaledBoxExtent();
        CollectionParameterHelpers::BoxSidesFiller(boxParameters, boxExtent);
        _collectionBoxes.Add(MoveTemp(boxParameters));

        box->OnComponentBeginOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementBeginOverlap);
        box->OnComponentEndOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementEndOverlap);

        _collectionElements.Emplace(box);
    }
}

void UCollisionsCollectionComponent::_collectCapsules() {
    TArray<UActorComponent*> const capsuleActors = GetOwner()->GetComponentsByTag(UCapsuleComponent::StaticClass(), _collectionTag);
    _collectionBoxes.Reserve(capsuleActors.Num());

    for (auto const capsuleActor : capsuleActors) {
        auto const capsule = Cast<UCapsuleComponent>(capsuleActor);
        check(capsule);

        auto const collisionProfile = capsule->GetCollisionProfileName();
        if (collisionProfile != _collectionCollisionProfileName) {
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Capsule collision tagged as %s has Profile %s instead of %s, replacing."), *_collectionTag.ToString(),
                *collisionProfile.ToString(), *_collectionCollisionProfileName.ToString());

            capsule->SetCollisionProfileName(_collectionCollisionProfileName);
        }

        bool const generateOverlapEvents = capsule->GetGenerateOverlapEvents();
        if (generateOverlapEvents != _generateOverlapEvents) {
            auto const collisionString = generateOverlapEvents ? TEXT("true") : TEXT("false");
            auto const collectionString = _generateOverlapEvents ? TEXT("true") : TEXT("false");
            UE_LOG(LogTemp, Warning, WFUNCTION TEXT(" Capsule collision has \"generate overlap events\" set to %s instead of %s, replacing."), collisionString,
                collectionString);

            capsule->SetGenerateOverlapEvents(_generateOverlapEvents);
        }

        float const halfHeight = capsule->GetScaledCapsuleHalfHeight();
        float const radius = capsule->GetScaledCapsuleRadius();

        auto capsuleParameters = FCollectionCapsuleParameters();
        FCollectionBoxParameters& box = capsuleParameters.CylinderBox;
        box.RootRelativeTransform = capsule->GetRelativeTransform();
        auto const boxExtent = FVector(radius, radius, halfHeight);
        CollectionParameterHelpers::BoxSidesFiller(box, boxExtent);

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
    UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, FHitResult const& sweepResult) {
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
