// Fill out your copyright notice in the Description page of Project Settings.

#include "ConeOfColdSkill.h"

#include "MaterialsContainerComponent.h"
#include "ThermodynamicComponent.h"

void UConeOfColdSkill::ExecuteCast(const TObjectPtr<AActor> caster) {
	// Capsule component's setup
	_cone = NewObject<UThermodynamicComponent>(caster, TEXT("Cone Of Cold ThermodynamicC"));
	_cone->SetupAttachment(caster->GetRootComponent());
	_cone->SetRelativeLocation(FVector::ForwardVector * _height);
	_cone->SetRelativeRotation(FRotator{ 90., 0., 0. });
	_cone->SetCapsuleHalfHeight(_height);
	_cone->SetCapsuleRadius(_radius);
	_cone->bMultiBodyOverlap = true;

	_cone->RegisterComponent();

	if (const auto pawn = Cast<APawn>(caster)) {
		_casterController = pawn->GetController();
		if (_casterController->IsA)
	}

	_startCooldown();
}

void UConeOfColdSkill::ExecuteChannelingTick(float deltaTime, const TObjectPtr<AActor> caster) {
	


	DrawDebugCapsule(GetWorld(), _cone->GetComponentLocation(), _cone->GetUnscaledCapsuleHalfHeight(), _cone->GetUnscaledCapsuleRadius(),
		_cone->GetComponentRotation().Quaternion(), FColor::Cyan, false);
}

void UConeOfColdSkill::AbortChanneling() {

}

#if DO_CHECK
void UConeOfColdSkill::CheckParametersSanity() const {

}
#endif
