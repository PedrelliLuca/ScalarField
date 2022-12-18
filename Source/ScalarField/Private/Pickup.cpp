// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

APickup::APickup() {
	_meshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	_meshC->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	SetRootComponent(_meshC);

	_interactableC = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));
	_interactableC->SetInteractableNameText(FText::FromString("Pickup"));
	_interactableC->SetInteractableActionText(FText::FromString("Take"));
	_interactableC->SetupAttachment(_meshC);

	_pickupC = CreateDefaultSubobject<UPickupComponent>(TEXT("Pickup Component"));
}

void APickup::BeginPlay() {
	Super::BeginPlay();

	if (bNetStartup) {
		// The actor was loaded directly from the map
		_pickupC->InitializePickup();
	}

	if (!bNetStartup) {
		_alignWithGround();
	}
}

void APickup::_alignWithGround() {
}
