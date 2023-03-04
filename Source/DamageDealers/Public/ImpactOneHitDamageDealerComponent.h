// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"

#include "ImpactOneHitDamageDealerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DAMAGEDEALERS_API UImpactOneHitDamageDealerComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UImpactOneHitDamageDealerComponent();

	TObjectPtr<UCapsuleComponent> GetImpactCapsule() { return _impactCapsule; }

	void AddImmuneActor(TWeakObjectPtr<AActor> immuneActor) {
		_hitActors.Emplace(immuneActor);
	}

protected:
	void BeginPlay() override;

private:
	UFUNCTION()
	void _onImpactBegin(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);
	
	UPROPERTY(EditAnywhere, NoClear)
	TObjectPtr<UCapsuleComponent> _impactCapsule;

	UPROPERTY(EditAnywhere)
	double _impactDamage = 50.0;

	TSet<TWeakObjectPtr<AActor>> _hitActors;
};
