// Fill out your copyright notice in the Description page of Project Settings.

#include "Doors/RotatingDoor.h"

#include "InteractorPlayerComponent.h"
#include "InventoryInterface.h"

ARotatingDoor::ARotatingDoor() {
    PrimaryActorTick.bCanEverTick = false;

    _sceneC = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(_sceneC);

    _rotatingMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatingMesh"));
    _rotatingMeshC->SetupAttachment(GetRootComponent());

    _interactableC = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
    _interactableC->SetupAttachment(GetRootComponent());

    _openingsC = CreateDefaultSubobject<UOpeningsInteractionComponent>(TEXT("OpeningsComponent"));
}

void ARotatingDoor::BeginPlay() {
    Super::BeginPlay();

    auto const rotationParams = FOpeningParameters{_timeToOpen, _openRotation, _closedRotation};
    _openingsC->SetOpening(_rotatingMeshC, rotationParams);
    _openingsC->OnOpeningStateChange().AddUObject(this, &ARotatingDoor::_onOpeningPhaseChange);

    _interactableC->OnInteraction().AddDynamic(this, &ARotatingDoor::_onDoorInteractedBy);
}

void ARotatingDoor::_onDoorInteractedBy(TScriptInterface<IInteractor> interactor) {
    auto const concreteInteractor = Cast<UInteractorPlayerComponent>(interactor.GetObject());
    check(IsValid(concreteInteractor));

    auto const interactingController = Cast<AController>(concreteInteractor->GetOwner());
    check(IsValid(interactingController));

    auto const inventoryComponent = Cast<IInventory>(interactingController->GetPawn()->FindComponentByInterface(UInventory::StaticClass()));
    if (inventoryComponent == nullptr) {
        interactor->EndInteraction();
        return;
    }

    if (inventoryComponent->FindItemByClass(_compatibleKey) != nullptr) {
        _openingsC->Toggle();
    }

    interactor->EndInteraction();
}

void ARotatingDoor::_onOpeningPhaseChange(EOpeningPhase const newOpeningState) {
    auto actionText = FText{};

    switch (newOpeningState) {
        case EOpeningPhase::Closed:
        case EOpeningPhase::Closing:
            actionText = FText::FromString(FString{"Open"});
            break;
        case EOpeningPhase::Open:
        case EOpeningPhase::Opening:
            actionText = FText::FromString(FString{"Close"});
            break;
        default:
            checkNoEntry();
    }

    _interactableC->SetInteractableActionText(actionText);
}
