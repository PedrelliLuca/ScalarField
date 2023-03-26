// Fill out your copyright notice in the Description page of Project Settings.

#include "ScalarFieldPickup.h"

AScalarFieldPickup::AScalarFieldPickup() {
    _meshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
    _meshC->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

    SetRootComponent(_meshC);

    _interactableC = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));
    _interactableC->SetInteractableNameText(FText::FromString("Pickup"));
    _interactableC->SetInteractableActionText(FText::FromString("Take"));
    _interactableC->SetupAttachment(_meshC);

    _pickupC = CreateDefaultSubobject<UPickupComponent>(TEXT("Pickup Component"));
}

void AScalarFieldPickup::InitializePickup(TSubclassOf<UObject> itemClass, int32 quantity) {
    _pickupC->InitializePickup(itemClass, quantity);
}

#if WITH_EDITOR
void AScalarFieldPickup::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
    Super::PostEditChangeProperty(propertyChangedEvent);

    const auto property = propertyChangedEvent.Property;
    const auto propertyName = property != nullptr ? property->GetFName() : NAME_None;

    // If a new pickup is selected in the property editor, change the mesh to reflect the new item being selected
    if (propertyName == GET_MEMBER_NAME_CHECKED(AScalarFieldPickup, _itemTemplate) && IsValid(_itemTemplate)) {
        _meshC->SetStaticMesh(_itemTemplate->GetMesh());
    }
}
#endif

void AScalarFieldPickup::BeginPlay() {
    Super::BeginPlay();

    if (bNetStartup) {
        // The IPickup was loaded directly from the map => we initialize it using the template set by the designers.
        // On the contrary, if the IPickup was created during play, we don't want to use the item template: we expect
        // its creator, whoever it is, to call InitializePickup() explicitly and provide the class and quantity it wants.
        InitializePickup(_itemTemplate->GetClass(), _itemTemplate->GetQuantity());
    } else {
        _alignWithGround();
    }
}

void AScalarFieldPickup::_alignWithGround() {
}
