// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "CollisionsCollectionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FCollectionPlayBegunSignature);
DECLARE_MULTICAST_DELEGATE_SixParams(FCollectionBeginOverlapSignature, UPrimitiveComponent*, AActor*, UPrimitiveComponent*, int32, bool, const FHitResult&);
DECLARE_MULTICAST_DELEGATE_FourParams(FCollectionEndOverlapSignature, UPrimitiveComponent*, AActor*, UPrimitiveComponent*, int32);

class UCollisionsCollectionSubsystem;

struct FCollectionSphereParameters {
    FTransform RootRelativeTransform = FTransform::Identity;
    float Radius = 0.0f;
};

struct FCollectionBoxParameters {
    FTransform RootRelativeTransform = FTransform::Identity;
    FBox2D Box;
};

/**
 * \brief Collects Sphere Collisions with a given tag so that they can look as a single non trivial collision for clients.
 */
UCLASS(ClassGroup = "Collision", meta = (BlueprintSpawnableComponent))
class COLLISIONSCOLLECTION_API UCollisionsCollectionComponent : public UActorComponent {
    GENERATED_BODY()

    friend class UCollisionsCollectionSubsystem;

public:
    UCollisionsCollectionComponent();

    // Queries world and updates overlap tracking state for each component of this collection.
    void UpdateOverlaps();

    // Returns unique list of components this collection is overlapping with.
    void GetOverlappingComponents(TSet<UPrimitiveComponent*>& componentsOverlappingCollection) const;

    bool HasElement(const UPrimitiveComponent* collectionElement) const;

    const TArray<FCollectionSphereParameters>& GetCollectionSpheres() const;

    FCollectionPlayBegunSignature OnCollectionPlayBegun;

    FCollectionBeginOverlapSignature OnCollectionBeginOverlap;
    FCollectionEndOverlapSignature OnCollectionEndOverlap;

    void BeginDestroy() override;

protected:
    void BeginPlay() override;

private:
    void _collectSpheres();
    void _collectBoxes();
    // TODO: _collectCapsules()

    UFUNCTION()
    void _collectionElementBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex,
        bool bFromSweep, const FHitResult& sweepResult);

    UFUNCTION()
    void _collectionElementEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

    /*void _beginCollectionOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex,
        bool bFromSweep, const FHitResult& sweepResult);*/

    // The Tag the collisions must have to be registered by the collection.
    UPROPERTY(EditAnywhere, Category = "Collection Parameters")
    FName _collectionTag;

    // The Collision Profile the tagged collisions must have to be registered by the collection.
    UPROPERTY(EditAnywhere, Category = "Collection Parameters")
    FName _collectionCollisionProfileName;

    // Whether the members of the collection should generate overlap events.
    UPROPERTY(EditAnywhere, Category = "Collection Parameters")
    bool _generateOverlapEvents;

    // For cache-friendly, fast iterations.
    TArray<FCollectionSphereParameters> _collectionSpheres;
    TArray<FCollectionBoxParameters> _collectionBoxes;

    // Non cache-friendly, meant exclusively for internal use. Needed because:
    // 1. The collection needs to provide some functions analogous to USceneComponent ones (e.g. UpdateOverlaps())
    // 2. We don't want collection elements to fire overlap events with each other.
    TSet<TWeakObjectPtr<UPrimitiveComponent>> _collectionElements;

    // Given another collection and one of its elements, tells the number of elements of this collection currently overlapping with it.
    TMap<TWeakObjectPtr<UCollisionsCollectionComponent>, TMap<TWeakObjectPtr<UPrimitiveComponent>, int>> _overlapsWithOtherCollectionElements;

    TWeakObjectPtr<UCollisionsCollectionSubsystem> _subsystem;
};
