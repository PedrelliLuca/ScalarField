// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldPlayerController.h"

#include "InventoryManipulationSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "NewStateComponent.h"
#include "SkillsContainerComponent.h"
#include "TacticalPauseWorldSubsystem.h"

AScalarFieldPlayerController::AScalarFieldPlayerController() {
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    _movementCommandC = CreateDefaultSubobject<UPlayerMovementCommandComponent>(TEXT("Movement Command Component"));
    _stateC = CreateDefaultSubobject<UStateComponent>(TEXT("State Component"));
    _interactorC = CreateDefaultSubobject<UInteractorPlayerComponent>(TEXT("Interactor Component"));
    _widgetsPresenterC = CreateDefaultSubobject<UWidgetsPresenterComponent>(TEXT("Widgets Presenter"));
}

void AScalarFieldPlayerController::PlayerTick(const float deltaTime) {
    Super::PlayerTick(deltaTime);

    if (!_bNewSkillSystem) {
        // Generally speaking, the tick of the states should stop if the tactical pause is active. However, some states
        // are special and are not affected by it.
        if (!_bIsTacticalPauseOn || !_stateC->IsCurrentStateAffectedByPause()) {
            _stateC->PerformTickBehavior(deltaTime);
        }

        // Tick of movement commands never occurs during the tactical pause.
        if (!_bIsTacticalPauseOn) {
            _movementCommandC->MovementTick(deltaTime);
        }
    }
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
    InputComponent->BindAction("AbortCast", IE_Pressed, this, &AScalarFieldPlayerController::_onSkillAbort);
    InputComponent->BindAction("SetTarget", IE_Released, this, &AScalarFieldPlayerController::_onSetTargetPressed);

    InputComponent->BindAction("Interact", IE_Pressed, this, &AScalarFieldPlayerController::_onInteraction);

    InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AScalarFieldPlayerController::_onInventoryToggle);

    InputComponent->BindAction("ToggleTacticalPause", IE_Released, this, &AScalarFieldPlayerController::_onTacticalPauseToggled);
}

void AScalarFieldPlayerController::BeginPlay() {
    Super::BeginPlay();

    const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
    pauseSubsys->OnTacticalPauseToggle().AddUObject(this, &AScalarFieldPlayerController::_answerTacticalPauseToggle);
    _bIsTacticalPauseOn = pauseSubsys->IsTacticalPauseOn();

    const auto inventorySubsys = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInventoryManipulationSubsystem>();
    inventorySubsys->SetHUDToShowOnClose(_widgetsPresenterC->GetHUDWidget());
    inventorySubsys->SetInventoryContainerWidget(_widgetsPresenterC->GetInventoryContainerWidget());

    if (_bNewSkillSystem) {
        _movementCommandC->SetDefaultMovementMode();
    }
}

void AScalarFieldPlayerController::_onSetDestinationPressed() {
    if (!_bNewSkillSystem) {
        _movementCommandC->StopMovement();
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        FHitResult hit;
        GetHitResultUnderCursor(ECC_Visibility, true, hit);
        stateC->TrySetMovementDestination(hit.Location);
    }
}

void AScalarFieldPlayerController::_onSetDestinationReleased() {
    // We look for the location in the world where the player has pressed the input
    FHitResult hit;
    GetHitResultUnderCursor(ECC_Visibility, true, hit);

    if (!_bNewSkillSystem) {
        _movementCommandC->SetDestination(hit.Location);
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TrySetMovementDestination(hit.Location);
    }
}

void AScalarFieldPlayerController::_onSetTargetPressed() {
    FHitResult hit;
    GetHitResultUnderCursor(ECC_Visibility, true, hit);
    if (!_bNewSkillSystem) {
        _stateC->PerformTargetingBehavior(hit.GetActor());
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        // We don't know what the skill we want to cast is going to need, so we'll provide it with everything we have.
        FSkillTargetPacket targetPacket;
        targetPacket.TargetActor = hit.GetActor();
        targetPacket.TargetLocation = hit.Location;

        stateC->TrySetSkillTarget(targetPacket);
    }
}

void AScalarFieldPlayerController::_onSkill1Cast() {
    constexpr int32 skillKey = 1;

    if (!_bNewSkillSystem) {
        const auto skillsContainer = GetPawn()->FindComponentByClass<USkillsContainerComponent>();
        check(IsValid(skillsContainer));

        auto skill = skillsContainer->GetSkillAtIndex(_getSkillIdxFromKey(skillKey));
        if (!IsValid(skill)) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): No skill bound"), *FString{__FUNCTION__});
            return;
        }

        _stateC->PerformSkillExecutionBehavior(MoveTemp(skill));
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    }
}

void AScalarFieldPlayerController::_onSkill2Cast() {
    constexpr int32 skillKey = 2;

    if (!_bNewSkillSystem) {
        const auto skillsContainer = GetPawn()->FindComponentByClass<USkillsContainerComponent>();
        check(IsValid(skillsContainer));

        auto skill = skillsContainer->GetSkillAtIndex(_getSkillIdxFromKey(skillKey));
        if (!IsValid(skill)) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): No skill bound"), *FString{__FUNCTION__});
            return;
        }

        _stateC->PerformSkillExecutionBehavior(MoveTemp(skill));
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    }
}

void AScalarFieldPlayerController::_onSkill3Cast() {
    constexpr int32 skillKey = 3;

    if (!_bNewSkillSystem) {
        const auto skillsContainer = GetPawn()->FindComponentByClass<USkillsContainerComponent>();
        check(IsValid(skillsContainer));

        auto skill = skillsContainer->GetSkillAtIndex(_getSkillIdxFromKey(skillKey));
        if (!IsValid(skill)) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): No skill bound"), *FString{__FUNCTION__});
            return;
        }

        _stateC->PerformSkillExecutionBehavior(MoveTemp(skill));
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    }
}

void AScalarFieldPlayerController::_onSkill4Cast() {
    constexpr int32 skillKey = 4;

    if (!_bNewSkillSystem) {
        const auto skillsContainer = GetPawn()->FindComponentByClass<USkillsContainerComponent>();
        check(IsValid(skillsContainer));

        auto skill = skillsContainer->GetSkillAtIndex(_getSkillIdxFromKey(skillKey));
        if (!IsValid(skill)) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): No skill bound"), *FString{__FUNCTION__});
            return;
        }

        _stateC->PerformSkillExecutionBehavior(MoveTemp(skill));
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    }
}

void AScalarFieldPlayerController::_onSkill5Cast() {
    constexpr int32 skillKey = 5;

    if (!_bNewSkillSystem) {
        const auto skillsContainer = GetPawn()->FindComponentByClass<USkillsContainerComponent>();
        check(IsValid(skillsContainer));

        auto skill = skillsContainer->GetSkillAtIndex(_getSkillIdxFromKey(skillKey));
        if (!IsValid(skill)) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): No skill bound"), *FString{__FUNCTION__});
            return;
        }

        _stateC->PerformSkillExecutionBehavior(MoveTemp(skill));
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TryCastSkillAtIndex(_getSkillIdxFromKey(skillKey));
    }
}

void AScalarFieldPlayerController::_onSkillAbort() {
    if (!_bNewSkillSystem) {
        _stateC->PerformAbortBehavior();
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TryAbortSkillInExecution();
    }
}

void AScalarFieldPlayerController::_onInteraction() {
    if (!_bNewSkillSystem) {
        _stateC->PerformInteractionBehavior();
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TryInteracting();
    }
}

void AScalarFieldPlayerController::_onInventoryToggle() {
    if (!_bNewSkillSystem) {
        _stateC->PerformInventoryToggleBehavior();
    } else {
        const auto stateC = GetPawn()->FindComponentByClass<UNewStateComponent>();
        check(IsValid(stateC));

        stateC->TryToggleInventory();
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

constexpr int32 AScalarFieldPlayerController::_getSkillIdxFromKey(const int32 key) {
    // keys [1, 2, ..., 9, 0] => indices [0, 1, ..., 8, 9]
    return key != 0 ? key - 1 : KEY_ASSIGNABLE_SKILLS - 1;
}
