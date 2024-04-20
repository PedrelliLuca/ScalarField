// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionsCollectionComponent.h"

#include "Components/SphereComponent.h"

// https://stackoverflow.com/questions/14421656/is-there-widely-available-wide-character-variant-of-file
#define WIDE2(X) L##X
#define WIDE1(X) WIDE2(X)
#define WFUNCTION WIDE1(__FUNCTION__)

UCollisionsCollectionComponent::UCollisionsCollectionComponent()
    : _collectionTag("")
    , _collectionCollisionProfileName("")
    , _collectionSpheres()
    , _overlappingElements(0) {
    PrimaryComponentTick.bCanEverTick = false;
}

void UCollisionsCollectionComponent::BeginPlay() {
    Super::BeginPlay();

    _collectSpheres();
}

void UCollisionsCollectionComponent::_collectSpheres() {
    const TArray<UActorComponent*> sphereActors = GetOwner()->GetComponentsByTag(USphereComponent::StaticClass(), _collectionTag);
    _collectionSpheres.Reserve(sphereActors.Num());

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
        sphereParameters.RootRelativeLocation = FVector2D(sphere->GetComponentTransform().GetLocation());
        sphereParameters.Radius = sphere->GetScaledSphereRadius();
        _collectionSpheres.Add(MoveTemp(sphereParameters));

        sphere->OnComponentBeginOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementBeginOverlap);
        sphere->OnComponentEndOverlap.AddDynamic(this, &UCollisionsCollectionComponent::_collectionElementEndOverlap);
    }
}

void UCollisionsCollectionComponent::_collectionElementBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
    UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
    // If _overlappingElements was 0 fire event saying collection is now overlapping. Then, increase _overlappingElements.
}

void UCollisionsCollectionComponent::_collectionElementEndOverlap(
    UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
    // Decrease _overlappingElements. If counter is now 0 fire event saying collection is not overlapping anymore
}
