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
	_impactCapsule->OnComponentBeginOverlap.AddDynamic(this, &UImpactOneHitDamageDealerComponent::_tryApplyImpulse);
}

void UImpactOneHitDamageDealerComponent::_tryApplyImpulse(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult & sweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("%s() call!"), *FString{__FUNCTION__});

	// This component cannot damage its owner nor actors that have already been damaged.
	if (otherActor == GetOwner() || _hitActors.Contains(otherActor)) {
		return;
	}

	/*const auto impulsePlane = FPlane{ 0.f, 0.f, 1.f, _impulseCenter.Z };
	const auto otherLocationProj = FVector::PointPlaneProject(otherActor->GetActorLocation(), impulsePlane);*/

	const auto otherLocation = otherActor->GetActorLocation();
	const auto otherProjectedLocation = FVector{ otherLocation.X, otherLocation.Y, _impulseCenter.Z };
	const auto impulseDirection = (otherProjectedLocation - _impulseCenter).GetSafeNormal();

	otherComp->AddImpulse(impulseDirection * _impulseIntensity, NAME_None, true);

	// TODO: create and check for the UImpactDamageHandlerComponent on otherActor, then call HandleDamage on it. That
	// component will check for resistances, immunities and so on before calling AActor::TakeDamage. That's a bit
	// overkill at the moment, so we'll simply call TakeDamage

	const TSubclassOf<UDamageType> damageType = UImpactDamageType::StaticClass();
	const FDamageEvent damageEvent{ damageType };
	otherActor->TakeDamage(_impactDamage, damageEvent, nullptr, GetOwner());
	
	_hitActors.Emplace(otherActor);
}
