// Fill out your copyright notice in the Description page of Project Settings.

#include "ImpactOneHitDamageDealerComponent.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/MovementComponent.h"
#include "ImpactDamageHandlerInterface.h"

UImpactOneHitDamageDealerComponent::UImpactOneHitDamageDealerComponent() {
    PrimaryComponentTick.bCanEverTick = false;
    bAutoActivate = true;

    _impactCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Impact Capsule"));
    _impactCapsule->bMultiBodyOverlap = true;
    _impactCapsule->SetNotifyRigidBodyCollision(true);
}

void UImpactOneHitDamageDealerComponent::BeginPlay() {
    Super::BeginPlay();
    _impactCapsule->OnComponentBeginOverlap.AddDynamic(this, &UImpactOneHitDamageDealerComponent::_tryApplyImpulse);
}

void UImpactOneHitDamageDealerComponent::_tryApplyImpulse(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp,
    int32 otherBodyIndex, bool bFromSweep, FHitResult const& sweepResult) {
    // This component cannot damage its owner nor actors that have already been damaged.
    if (otherActor == GetOwner() || _hitActors.Contains(otherActor)) {
        return;
    }

    auto const impactHandler = Cast<IImpactDamageHandler>(otherActor->FindComponentByInterface((UImpactDamageHandler::StaticClass())));
    if (impactHandler == nullptr) {
        // The hit actor doesn't know how to handle impact damage
        return;
    }

    auto const otherLocation = otherActor->GetActorLocation();
    auto const otherProjectedLocation = FVector{otherLocation.X, otherLocation.Y, _impulseCenter.Z};
    auto const impulseDirection = (otherProjectedLocation - _impulseCenter).GetSafeNormal();

    auto const velocity = impulseDirection * _impulseIntensity;

    impactHandler->HandleImpact(velocity, _impactDamage);

    _hitActors.Emplace(otherActor);
}
