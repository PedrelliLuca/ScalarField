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

#if WITH_EDITOR
void APickup::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
	Super::PostEditChangeProperty(propertyChangedEvent);

	const auto property = propertyChangedEvent.Property;
	const auto propertyName = property != nullptr ? property->GetFName() : NAME_None;

	// If a new pickup is selected in the property editor, change the mesh to reflect the new item being selected
	if (propertyName == GET_MEMBER_NAME_CHECKED(APickup, _itemTemplate) && IsValid(_itemTemplate)) {
		_meshC->SetStaticMesh(_itemTemplate->GetMesh());
	}
}
#endif

void APickup::BeginPlay() {
	Super::BeginPlay();

	if (bNetStartup) {
		// The actor was loaded directly from the map
		_pickupC->InitializePickup(_itemTemplate);
	}

	if (!bNetStartup) {
		_alignWithGround();
	}
}

void APickup::_alignWithGround() {
}
