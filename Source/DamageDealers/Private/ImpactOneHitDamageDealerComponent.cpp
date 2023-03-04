// Fill out your copyright notice in the Description page of Project Settings.


#include "ImpactOneHitDamageDealerComponent.h"

#include "ImpactDamageType.h"
#include "Engine/DamageEvents.h"

UImpactOneHitDamageDealerComponent::UImpactOneHitDamageDealerComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;

	_impactCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Impact Capsule"));
	_impactCapsule->bMultiBodyOverlap = true;
	_impactCapsule->SetNotifyRigidBodyCollision(true);
}

void UImpactOneHitDamageDealerComponent::BeginPlay() {
	Super::BeginPlay();
	_impactCapsule->OnComponentHit.AddDynamic(this, &UImpactOneHitDamageDealerComponent::_onImpactBegin);
}

// TODO: at the moment this is properly called only on collisions with physics simulation ON. Find another solution based on overlap.
void UImpactOneHitDamageDealerComponent::_onImpactBegin(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit) {
	UE_LOG(LogTemp, Warning, TEXT("%s() call!"), *FString{__FUNCTION__});

	// This component cannot damage its owner nor actors that have already been damaged.
	if (otherActor == GetOwner() || _hitActors.Contains(otherActor)) {
		return;
	}

	// TODO: create and check for the UImpactDamageHandlerComponent on otherActor, then call HandleDamage on it. That
	// component will check for resistances, immunities and so on before calling AActor::TakeDamage. That's a bit
	// overkill at the moment, so we'll simply call TakeDamage

	const TSubclassOf<UDamageType> damageType = UImpactDamageType::StaticClass();
	const FDamageEvent damageEvent{ damageType };
	otherActor->TakeDamage(_impactDamage, damageEvent, nullptr, GetOwner());
	
	_hitActors.Emplace(otherActor);
}