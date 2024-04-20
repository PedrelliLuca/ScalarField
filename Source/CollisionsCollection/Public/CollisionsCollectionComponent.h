// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "CollisionsCollectionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_SixParams(FCollectionBeginOverlapSignature, UPrimitiveComponent*, AActor*, UPrimitiveComponent*, int32, bool, const FHitResult&);

struct FCollectionSphereParameters {
    FVector2D RootRelativeLocation = FVector2D::ZeroVector;
    float Radius = 0.0f;
};

/**
 * \brief Collects Sphere Collisions with a given tag so that they can act as one for other components.
 */
UCLASS(ClassGroup = "Collision", meta = (BlueprintSpawnableComponent))
class COLLISIONSCOLLECTION_API UCollisionsCollectionComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UCollisionsCollectionComponent();

protected:
    void BeginPlay() override;

private:
    void _collectSpheres();
    // TODO: _collectCapsules(), _collectBoxes()

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

    TArray<FCollectionSphereParameters> _collectionSpheres;

    // The number of elements in the collection which are currently overlapping
    int32 _overlappingElements;
};
