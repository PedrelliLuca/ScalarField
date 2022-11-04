// Fill out your copyright notice in the Description page of Project Settings.

#include "ConeOfColdSkill.h"

void UConeOfColdSkill::ExecuteCast(const TObjectPtr<AActor> caster) {
	// Capsule component setup
	_cone = NewObject<UThermodynamicComponent>(caster, TEXT("Cone Of Cold ThermodynamicC"));
	_cone->SetupAttachment(caster->GetRootComponent());
	_cone->SetRelativeLocation(FVector::ForwardVector * _height);
	_cone->SetRelativeRotation(FRotator{ 90., 0., 0. });
	_cone->SetCapsuleHalfHeight(_height);
	_cone->SetCapsuleRadius(_radius);
	_cone->bMultiBodyOverlap = true;

	// Thermodynamic component setup
	_cone->SetHeatCapacity(_coneHeatCapacity);
	_cone->SetTemperature(_coneTemperature, true);

	_cone->RegisterComponent();
	
	_startCooldown();
}

void UConeOfColdSkill::ExecuteChannelingTick(float deltaTime, const TObjectPtr<AActor> caster) {
	FlushPersistentDebugLines(GetWorld());
	
	DrawDebugCapsule(GetWorld(), _cone->GetComponentLocation(), _cone->GetUnscaledCapsuleHalfHeight(), _cone->GetUnscaledCapsuleRadius(),
		_cone->GetComponentRotation().Quaternion(), FColor::Cyan, false);
}

void UConeOfColdSkill::Abort() {
	Super::Abort();

	// Are you calling this function on a casted cone of cold?
	check(_cone.IsValid());

	_cone->DestroyComponent();
}

#if DO_CHECK
void UConeOfColdSkill::CheckParametersSanity() const {

}
#endif
