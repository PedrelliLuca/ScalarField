// Fill out your copyright notice in the Description page of Project Settings.

#include "EnvironmentCell.h"

#include "Components/StaticMeshComponent.h"
#include "EnvironmentCellSettings.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/DateTime.h"
#include "Misc/Timespan.h"

AEnvironmentCell::AEnvironmentCell() {
	PrimaryActorTick.bCanEverTick = true;

	const UEnvironmentCellSettings* const cellSettings = GetDefault<UEnvironmentCellSettings>();

	_isFrozen = false;

	// Box component setup. The box is the actual cell and its purpose is to ping the environment grid whenever the player character enters/leaves the cell
	_boxC = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(_boxC);

	// This is what makes the environment cell interact with the scalar field character
	_boxC->SetCollisionProfileName("EnvironmentCell");
	_boxC->OnComponentBeginOverlap.AddDynamic(this, &AEnvironmentCell::_onCellBeginningOverlap);
	_boxC->OnComponentEndOverlap.AddDynamic(this, &AEnvironmentCell::_onCellEndingOverlap);

	// I am not sure if _staticMeshC is here to stay or I'll get rid of it in the future
	_staticMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	_staticMeshC->SetupAttachment(_boxC);
	_staticMeshC->SetStaticMesh(cellSettings->GetCellPlaceholderMesh());
	_staticMeshC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_staticMeshC->SetCastShadow(false);
	_materialInstance = _staticMeshC->CreateDynamicMaterialInstance(0, cellSettings->GetCellPlaceholderMaterial(), TEXT("Dynamic Placeholder Material"));
}

void AEnvironmentCell::FreezeTime() {
	if (_isFrozen) {
		return;
	}
	_freezingTime = FDateTime::Now();
	
	_isFrozen = true;
	SetActorTickEnabled(!_isFrozen);
	_materialInstance->SetVectorParameterValue(TEXT("ActivationColor"), GetDefault<UEnvironmentCellSettings>()->GetCellFrozenColor());
}

void AEnvironmentCell::UnfreezeTime() {
	if (!_isFrozen) {
		return;
	}

	// TODO: to be used in the future for gameplay logic
	FTimespan freezeTimeSpan = FDateTime::Now() - _freezingTime;

	_isFrozen = false;
	SetActorTickEnabled(!_isFrozen);
	_materialInstance->SetVectorParameterValue(TEXT("ActivationColor"), GetDefault<UEnvironmentCellSettings>()->GetCellUnfrozenColor());
}

void AEnvironmentCell::_onCellBeginningOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// Because of how collisions have been set up, OtherComp has object channel GridInteractingPawn
	// UE_LOG(LogTemp, Warning, TEXT("Cell %s _onCellBeginningOverlap"), *_coordinates.ToString());
	OnCellBeginningOverlap.Broadcast(_coordinates);
}

void AEnvironmentCell::_onCellEndingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	// Because of how collisions have been set up, OtherComp has object channel GridInteractingPawn
	// UE_LOG(LogTemp, Warning, TEXT("Cell %s _onCellEndingOverlap"), *_coordinates.ToString());
	OnCellEndingOverlap.Broadcast(_coordinates);
}