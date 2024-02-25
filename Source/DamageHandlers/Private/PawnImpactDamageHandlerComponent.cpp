// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnImpactDamageHandlerComponent.h"

#include "GameFramework/PawnMovementComponent.h"
#include "HealthComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"

UPawnImpactDamageHandlerComponent::UPawnImpactDamageHandlerComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UPawnImpactDamageHandlerComponent::BeginPlay() {
    Super::BeginPlay();

    if (APawn* const pawn = Cast<APawn>(GetOwner()); IsValid(pawn)) {
        if (_healthC = pawn->FindComponentByClass<UHealthComponent>(); _healthC.IsValid()) {
            AController* const controller = pawn->GetController();
            check(IsValid(controller));

            _pathFollowingC = controller->FindComponentByClass<UPathFollowingComponent>();
            if (!_pathFollowingC.IsValid()) {
                UE_LOG(LogTemp, Warning, TEXT("%s: No PathFollowingComponent on owner, ImpactDamage won't abort movement. Is this intended?"),
                    *FString(__FUNCTION__));
            }

            _navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(controller->GetWorld()); 
            if (!_navSys.IsValid()) {
                UE_LOG(LogTemp, Warning, TEXT("%s: No NavSysV1 on owner's controller, ImpactDamage won't change pawn's velocity. Is this intended?"),
                    *FString(__FUNCTION__));
            }

            _pawnMovementC = pawn->FindComponentByClass<UPawnMovementComponent>();
            if (!_pawnMovementC.IsValid()) {
                UE_LOG(LogTemp, Warning, TEXT("%s: No PawnMovementComponent on owner, ImpactDamage won't change pawn's velocity. Is this intended?"),
                    *FString(__FUNCTION__));
            }

        } else {
            UE_LOG(LogTemp, Error, TEXT("%s: Owner does not have a valid UHealthComponent and won't receive damage by impacts."), *FString(__FUNCTION__));
        }
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Owner is not an APawn, ImpactDamage is disabled."), *FString(__FUNCTION__));
    }
}

void UPawnImpactDamageHandlerComponent::HandleImpact(const FVector& velocity, const float damage) {
    check(_healthC.IsValid());
    _healthC->TakeDamage(damage);

    if (_pathFollowingC.IsValid() && _navSys.IsValid() && _pathFollowingC->GetStatus() != EPathFollowingStatus::Idle) {
        _pathFollowingC->AbortMove(*_navSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest, FAIRequestID::AnyRequest,
            EPathFollowingVelocityMode::Reset);
    }

    if (_pawnMovementC.IsValid()) {
        _pawnMovementC->Velocity = velocity;
    }
}
