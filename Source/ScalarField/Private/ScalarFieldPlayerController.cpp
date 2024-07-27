// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldPlayerController.h"

#include "GameFramework/PawnMovementComponent.h"
#include "InventoryManipulationSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "NewStateComponent.h"
#include "PlayerInputData.h"
#include "SkillsContainerComponent.h"
#include "TacticalPauseWorldSubsystem.h"
#include "ThermodynamicsSubsystem.h"

AScalarFieldPlayerController::AScalarFieldPlayerController() {
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    _movementCommandC = CreateDefaultSubobject<UPlayerMovementCommandComponent>(TEXT("Movement Command Component"));
    _interactorC = CreateDefaultSubobject<UInteractorPlayerComponent>(TEXT("Interactor Component"));
    _widgetsPresenterC = CreateDefaultSubobject<UWidgetsPresenterComponent>(TEXT("Widgets Presenter"));
}

void AScalarFieldPlayerController::SetupInputComponent() {
    // set up gameplay key bindings
    Super::SetupInputComponent();

    InputComponent->BindAction("SetDestination", IE_Pressed, this, &AScalarFieldPlayerController::_onSetDestinationPressed);
    InputComponent->BindAction("SetDestination", IE_Released, this, &AScalarFieldPlayerController::_onSetDestinationReleased);

    InputComponent->BindAction("Skill1Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill1Cast);
    InputComponent->BindAction("Skill2Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill2Cast);
    InputComponent->BindAction("Skill3Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill3Cast);
    InputComponent->BindAction("Skill4Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill4Cast);
    InputComponent->BindAction("Skill5Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill5Cast);
    InputComponent->BindAction("Skill6Cast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkill6Cast);

    InputComponent->BindAction("AbortCast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkillAbort);
    InputComponent->BindAction("SetTarget", IE_Released, this, &AScalarFieldPlayerController::_onSetTargetPressed);

    InputComponent->BindAction("Interact", IE_Pressed, this, &AScalarFieldPlayerController::_onInteraction);

    InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AScalarFieldPlayerController::_onInventoryToggle);

    InputComponent->BindAction("ToggleTacticalPause", IE_Released, this, &AScalarFieldPlayerController::_onTacticalPauseToggled);
    InputComponent->BindAction("ToggleHeatmapVisualization", IE_Released, this, &AScalarFieldPlayerController::_onHeatmapVisualizationToggled);
}

void AScalarFieldPlayerController::BeginPlay() {
    Super::BeginPlay();

    const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
    pauseSubsys->OnTacticalPauseToggle().AddUObject(this, &AScalarFieldPlayerController::_answerTacticalPauseToggle);
    _bIsTacticalPauseOn = pauseSubsys->IsTacticalPauseOn();

    if (const auto pawn = GetPawn(); IsValid(pawn)) {
        if (const auto healthC = pawn->FindComponentByClass<UHealthComponent>(); IsValid(healthC)) {
            healthC->OnDeath().AddUObject(this, &AScalarFieldPlayerController::_onControlledPawnDeath);
        }
    }

    _movementCommandC->SetDefaultMovementMode();

    _widgetsPresenterC->CreateAllWidgets();
    const auto inventorySubsys = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInventoryManipulationSubsystem>();
    inventorySubsys->SetHUDToShowOnClose(_widgetsPresenterC->GetHUDWidget());
    inventorySubsys->SetInventoryContainerWidget(_widgetsPresenterC->GetInventoryContainerWidget());

    if (const TObjectPtr<USkillsContainerComponent> skillsC = GetPawn()->FindComponentByClass<USkillsContainerComponent>(); IsValid(skillsC))
    {
        skillsC->CreateAllSkills();
        _widgetsPresenterC->ProvideSkillsToWidgets(skillsC);
    }
}

void AScalarFieldPlayerController::_onSetDestinationPressed() {
    // Very important: this must be called before each TrySetMovementDestination() call, point of failure.
    _movementCommandC->SetPlayerInputData(FPlayerInputData{IE_Pressed});

    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        FHitResult hit;
        GetHitResultUnderCursor(ECC_Visibility, true, hit);
        stateC->TrySetMovementDestination(hit.Location);
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onSetDestinationReleased() {
    // We look for the location in the world where the player has pressed the input
    FHitResult hit;
    GetHitResultUnderCursor(ECC_Visibility, true, hit);

    // Very important: this must be called before each TrySetMovementDestination() call, point of failure.
    _movementCommandC->SetPlayerInputData(FPlayerInputData{IE_Released});

    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TrySetMovementDestination(hit.Location);
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onSetTargetPressed() {
    FHitResult hit;
    GetHitResultUnderCursor(ECC_Visibility, true, hit);
    const auto pawn = GetPawn();
    if (const auto stateC = pawn->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        // We don't know what the skill we want to cast is going to need, so we'll provide it with everything we have.
        FSkillTargetPacket targetPacket;
        targetPacket.TargetActor = hit.GetActor();

        FVector mouseLoc;
        FVector mouseDir;
        DeprojectMousePositionToWorld(mouseLoc, mouseDir);
        const auto pawnPlane = FPlane{0.0f, 0.0f, 1.0f, pawn->GetActorLocation().Z};
        const auto pawnPlaneLocation = FMath::LinePlaneIntersection(mouseLoc, mouseLoc + mouseDir * PROJ_LINE_LENGTH, pawnPlane);
        targetPacket.TargetCasterPlaneLocation = pawnPlaneLocation;

        // TODO: compute targetPacket.TargetGroundLocation by projecting onto the ground somehow, this will be needed for skills like "Spawn Tree"

        stateC->TrySetSkillTarget(targetPacket);
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onSkill1Cast() {
    constexpr int32 skillKey = 1;

    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onSkill2Cast() {
    constexpr int32 skillKey = 2;

    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onSkill3Cast() {
    constexpr int32 skillKey = 3;

    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onSkill4Cast() {
    constexpr int32 skillKey = 4;

    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onSkill5Cast() {
    constexpr int32 skillKey = 5;

    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onSkill6Cast() {
    constexpr int32 skillKey = 6;

    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onSkillAbort() {
    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TryAbort();
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onInteraction() {
    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TryInteracting();
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onInventoryToggle() {
    if (const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>(); IsValid(stateC)) {
        stateC->TryToggleInventory();
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid UNewStateComponent"), *FString(__FUNCTION__));
    }
}

void AScalarFieldPlayerController::_onTacticalPauseToggled() {
    GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>()->ToggleWorldTacticalPauseStatus();
}

void AScalarFieldPlayerController::_answerTacticalPauseToggle(const bool bIsTacticalPauseOn, const double currentWorldTimeDilation) {
    /* Here we're literally overriding whatever the UTacticalPauseWorldSubsystem just did.
     * The PlayerController must never, ever, have its time dilation close to zero, since
     * that would cause the player to not be able to send any kind of input. */
    CustomTimeDilation = 1. / currentWorldTimeDilation;

    _bIsTacticalPauseOn = bIsTacticalPauseOn;
}

void AScalarFieldPlayerController::_onHeatmapVisualizationToggled() {
    UThermodynamicsSubsystem* thermoSubsys = GetWorld()->GetSubsystem<UThermodynamicsSubsystem>();
    thermoSubsys->OnHeatmapVisualizationToggle.Broadcast();
}

void AScalarFieldPlayerController::_onControlledPawnDeath(const TObjectPtr<AActor> deadActor) {
    // This stuff shouldn't be in the Player Controller!! It should be on a component located on the pawn.
    // Thing is, you'd have to port both the state machine and the movement system to the pawn for the above to happen.
    _onSkillAbort();
    DisableInput(this);
    _movementCommandC->SetMovementMode(EMovementCommandMode::MCM_Still);
    GetPawn()->GetMovementComponent()->StopMovementImmediately();
}

constexpr int32 AScalarFieldPlayerController::_getSkillIdxFromKey(const int32 key) {
    // keys [1, 2, ..., 9, 0] => indices [0, 1, ..., 8, 9]
    return key != 0 ? key - 1 : KEY_ASSIGNABLE_SKILLS - 1;
}
