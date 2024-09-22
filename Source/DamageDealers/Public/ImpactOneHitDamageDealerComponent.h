// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "CoreMinimal.h"

#include "ImpactOneHitDamageDealerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DAMAGEDEALERS_API UImpactOneHitDamageDealerComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UImpactOneHitDamageDealerComponent();

    TObjectPtr<UCapsuleComponent> GetImpactCapsule() { return _impactCapsule; }

    void AddImmuneActor(TWeakObjectPtr<AActor> immuneActor) { _hitActors.Emplace(immuneActor); }

    /** \brief The source point for the impulses hitting the actors overlapping the impact capsule */
    void SetImpulseCenter(FVector const& impulseCenter) { _impulseCenter = impulseCenter; }

protected:
    void BeginPlay() override;

private:
    UFUNCTION()
    void _tryApplyImpulse(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep,
        FHitResult const& sweepResult);

    UPROPERTY(EditAnywhere, NoClear)
    TObjectPtr<UCapsuleComponent> _impactCapsule;

    UPROPERTY(EditAnywhere)
    double _impactDamage = 50.0;

    UPROPERTY(EditAnywhere)
    double _impulseIntensity = 5.0;

    TSet<TWeakObjectPtr<AActor>> _hitActors;

    FVector _impulseCenter;
};
