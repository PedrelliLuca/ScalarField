// Fill out your copyright notice in the Description page of Project Settings.

#include "Doors/DoubleOpeningDoor.h"

#include "InteractorPlayerComponent.h"
#include "InventoryInterface.h"

ADoubleOpeningDoor::ADoubleOpeningDoor() {
    PrimaryActorTick.bCanEverTick = false;

    _sceneC = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(_sceneC);

    _leftOpeningMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftOpeningMesh"));
    _leftOpeningMeshC->SetupAttachment(GetRootComponent());
    _rightOpeningMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightOpeningMesh"));
    _rightOpeningMeshC->SetupAttachment(GetRootComponent());

    _interactableC = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
    _interactableC->SetupAttachment(GetRootComponent());

    _openingsC = CreateDefaultSubobject<UOpeningsInteractionComponent>(TEXT("OpeningsComponent"));
}

void ADoubleOpeningDoor::BeginPlay() {
    Super::BeginPlay();

    _openingsC->SetOpening(_leftOpeningMeshC, _leftOpeningParameters);
    _openingsC->SetOpening(_rightOpeningMeshC, _rightOpeningParameters);
    _openingsC->OnOpeningStateChange().AddUObject(this, &ADoubleOpeningDoor::_onOpeningPhaseChange);

    _interactableC->OnInteraction().AddDynamic(this, &ADoubleOpeningDoor::_onDoorInteractedBy);
}

void ADoubleOpeningDoor::_onDoorInteractedBy(TScriptInterface<IInteractor> interactor) {
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
        _openingsC->Toggle();
    }

    interactor->EndInteraction();
}

void ADoubleOpeningDoor::_onOpeningPhaseChange(const EOpeningPhase newOpeningState) {
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
