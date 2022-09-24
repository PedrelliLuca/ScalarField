// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentCell.h"

#include "Components/StaticMeshComponent.h"
#include "EnvironmentCellSettings.h"
#include "Materials/MaterialInstanceDynamic.h"

AEnvironmentCell::AEnvironmentCell() {
	PrimaryActorTick.bCanEverTick = true;

	const UEnvironmentCellSettings* const cellSettings = GetDefault<UEnvironmentCellSettings>();

	_side = cellSettings->GetCellSide();

	_boxC = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(_boxC);

	_staticMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	_staticMeshC->SetupAttachment(_boxC);
	_staticMeshC->SetStaticMesh(cellSettings->GetCellPlaceholderMesh());
	_staticMeshC->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_staticMeshC->SetCastShadow(false);
	_materialInstance = _staticMeshC->CreateDynamicMaterialInstance(0, cellSettings->GetCellPlaceholderMaterial(), TEXT("Dynamic Placeholder Material"));
}

void AEnvironmentCell::FreezeTime() {
	SetActorTickEnabled(false);
	_materialInstance->SetVectorParameterValue(TEXT("ActivationColor"), GetDefault<UEnvironmentCellSettings>()->GetCellFrozenColor());
}

void AEnvironmentCell::UnfreezeTime() {
	SetActorTickEnabled(true);
	_materialInstance->SetVectorParameterValue(TEXT("ActivationColor"), GetDefault<UEnvironmentCellSettings>()->GetCellUnfrozenColor());
}