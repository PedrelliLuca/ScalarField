// Fill out your copyright notice in the Description page of Project Settings.

#include "Doors/RotatingDoor.h"

#include "InteractorPlayerComponent.h"
#include "InventoryInterface.h"

ARotatingDoor::ARotatingDoor() {
    PrimaryActorTick.bCanEverTick = false;

    _rotatingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatingMesh"));
    SetRootComponent(_rotatingMesh);

    _interactableC = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
    _interactableC->SetupAttachment(GetRootComponent());

    _openingsC = CreateDefaultSubobject<UOpeningsInteractionComponent>(TEXT("OpeningsComponent"));
}

void ARotatingDoor::BeginPlay() {
    Super::BeginPlay();

    const auto rotationParams = FOpeningInteractionParameters{_timeToOpen, FRotator{0.0f, _yawToOpen, 0.0f}};
    _openingsC->SetOpening(_rotatingMesh, rotationParams);

    _interactableC->OnInteraction().AddDynamic(this, &ARotatingDoor::_onDoorInteractedBy);
}

void ARotatingDoor::_onDoorInteractedBy(TScriptInterface<IInteractor> interactor) {
    const auto concreteInteractor = Cast<UInteractorPlayerComponent>(interactor.GetObject());
    check(IsValid(concreteInteractor));

    const auto interactingController = Cast<AController>(concreteInteractor->GetOwner());
    check(IsValid(interactingController));

    const auto inventoryComponent = Cast<IInventory>(interactingController->GetPawn()->FindComponentByInterface(UInventory::StaticClass()));
    if (inventoryComponent == nullptr) {
        interactor->EndInteraction();
        return;
    }

    if (inventoryComponent->FindItemByClass(_compatibleKey) != nullptr) {
        _openingsC->Open();
    }

    interactor->EndInteraction();
}
