// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnImpactDamageHandlerComponent.h"

#include "GameFramework/PawnMovementComponent.h"
#include "ImpactDamageType.h"
#include "Navigation/PathFollowingComponent.h"
#include "NavigationSystem.h"

void UPawnImpactDamageHandlerComponent::HandleImpact(const FVector& velocity, const double damage, AActor* const damageCauser) {
	const auto owner = Cast<APawn>(GetOwner());
	// This component has the word "pawn" in its name for a reason...
	check(IsValid(owner));

	if (const auto controller = owner->GetController(); IsValid(controller)) {
		if (const auto pathFollowingC = controller->FindComponentByClass<UPathFollowingComponent>(); IsValid(pathFollowingC) && pathFollowingC->GetStatus() != EPathFollowingStatus::Idle) {
			if (const auto navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(controller->GetWorld()); IsValid(navSys)) {
				pathFollowingC->AbortMove(*navSys, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest
					, FAIRequestID::AnyRequest, EPathFollowingVelocityMode::Reset);
			}
		}
	}


	if (const auto movementC = owner->FindComponentByClass<UPawnMovementComponent>(); IsValid(movementC)) {
		movementC->Velocity = velocity;
	}

	const TSubclassOf<UDamageType> damageType = UImpactDamageType::StaticClass();
	const FDamageEvent damageEvent{ damageType };
	owner->TakeDamage(damage, damageEvent, nullptr, damageCauser);
}
