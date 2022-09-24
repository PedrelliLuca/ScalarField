// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentCell.h"

#include "Components/StaticMeshComponent.h"
#include "EnvironmentCellSettings.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ScalarFieldCharacter.h"

AEnvironmentCell::AEnvironmentCell() {
	PrimaryActorTick.bCanEverTick = true;

	const UEnvironmentCellSettings* const cellSettings = GetDefault<UEnvironmentCellSettings>();

	_side = cellSettings->GetCellSide();
	_isFrozen = true;

	_boxC = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(_boxC);
	_boxC->SetBoxExtent(FVector::OneVector * _side * 0.5);
	
	_boxC->OnComponentBeginOverlap.AddDynamic(this, &AEnvironmentCell::_onCellBeginningOverlap);
	_boxC->OnComponentEndOverlap.AddDynamic(this, &AEnvironmentCell::_onCellEndingOverlap);

	_staticMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	_staticMeshC->SetupAttachment(_boxC);
	_staticMeshC->SetStaticMesh(cellSettings->GetCellPlaceholderMesh());
	_staticMeshC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_staticMeshC->SetCastShadow(false);
	_materialInstance = _staticMeshC->CreateDynamicMaterialInstance(0, cellSettings->GetCellPlaceholderMaterial(), TEXT("Dynamic Placeholder Material"));
}

void AEnvironmentCell::FreezeTime() {
	_isFrozen = true;
	SetActorTickEnabled(_isFrozen);
	_materialInstance->SetVectorParameterValue(TEXT("ActivationColor"), GetDefault<UEnvironmentCellSettings>()->GetCellFrozenColor());
}

void AEnvironmentCell::UnfreezeTime() {
	_isFrozen = false;
	SetActorTickEnabled(_isFrozen);
	_materialInstance->SetVectorParameterValue(TEXT("ActivationColor"), GetDefault<UEnvironmentCellSettings>()->GetCellUnfrozenColor());
}

void AEnvironmentCell::_onCellBeginningOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (Cast<AScalarFieldCharacter>(OtherActor)) {
		UE_LOG(LogTemp, Warning, TEXT("Cell %s _onCellBeginningOverlap"), *_coordinates.ToString());
		OnCellBeginningOverlap.Broadcast(_coordinates);
	}
}

void AEnvironmentCell::_onCellEndingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (Cast<AScalarFieldCharacter>(OtherActor)) {
		UE_LOG(LogTemp, Warning, TEXT("Cell %s _onCellEndingOverlap"), *_coordinates.ToString());
		OnCellEndingOverlap.Broadcast(_coordinates);
	}
}