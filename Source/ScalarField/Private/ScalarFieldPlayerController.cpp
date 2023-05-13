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
    }

    // Tick of movement commands never occurs during the tactical pause.
    if (!_bIsTacticalPauseOn) {
        _movementCommandC->GetMovementCommand()->OnMovementTick(this, deltaTime);
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

    InputComponent->BindAction("Interact", IE_Pressed, _stateC.Get(), &UStateComponent::PerformInteractionBehavior);

    InputComponent->BindAction("ToggleInventory", IE_Pressed, _stateC.Get(), &UStateComponent::PerformInventoryToggleBehavior);

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
    _movementCommandC->GetMovementCommand()->OnStopMovement(this);
}

void AScalarFieldPlayerController::_onSetDestinationReleased() {
    _movementCommandC->GetMovementCommand()->OnSetDestination(this);
}

void AScalarFieldPlayerController::_onSetTargetPressed() {
    FHitResult hit;
    GetHitResultUnderCursor(ECC_Visibility, true, hit);
    _stateC->PerformTargetingBehavior(hit.GetActor());
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
    if (!_bNewSkillSystem) {
        const auto skillsContainer = GetPawn()->FindComponentByClass<USkillsContainerComponent>();
        check(IsValid(skillsContainer));

        constexpr int32 skillKey = 3;
        auto skill = skillsContainer->GetSkillAtIndex(_getSkillIdxFromKey(skillKey));
        if (!IsValid(skill)) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): No skill bound"), *FString{__FUNCTION__});
            return;
        }

        _stateC->PerformSkillExecutionBehavior(MoveTemp(skill));
    }
}

void AScalarFieldPlayerController::_onSkill4Cast() {
    if (!_bNewSkillSystem) {
        const auto skillsContainer = GetPawn()->FindComponentByClass<USkillsContainerComponent>();
        check(IsValid(skillsContainer));

        constexpr int32 skillKey = 4;
        auto skill = skillsContainer->GetSkillAtIndex(_getSkillIdxFromKey(skillKey));
        if (!IsValid(skill)) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): No skill bound"), *FString{__FUNCTION__});
            return;
        }

        _stateC->PerformSkillExecutionBehavior(MoveTemp(skill));
    }
}

void AScalarFieldPlayerController::_onSkill5Cast() {
    if (!_bNewSkillSystem) {
        const auto skillsContainer = GetPawn()->FindComponentByClass<USkillsContainerComponent>();
        check(IsValid(skillsContainer));

        constexpr int32 skillKey = 5;
        auto skill = skillsContainer->GetSkillAtIndex(_getSkillIdxFromKey(skillKey));
        if (!IsValid(skill)) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): No skill bound"), *FString{__FUNCTION__});
            return;
        }

        _stateC->PerformSkillExecutionBehavior(MoveTemp(skill));
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
