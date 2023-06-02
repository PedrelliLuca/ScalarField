// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryOpenState.h"

#include "InventoryManipulationSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SkillExecutionState.h"

void UInventoryOpenState::SetPawn(TObjectPtr<APawn> subjectPawn) {
    // Make sure that the pawn is not valid before setting it.
    if (ensureMsgf(!_subjectPawn.IsValid(), TEXT("State Subject can be set only once and has already been set."))) {
        check(IsValid(subjectPawn));
        _subjectPawn = MoveTemp(subjectPawn);

        _subjectStateC = _subjectPawn->FindComponentByClass<UNewStateComponent>();
        check(_subjectStateC.IsValid());
    }
}

void UInventoryOpenState::OnEnter() {
    const auto inventorySubsys = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInventoryManipulationSubsystem>();
    inventorySubsys->OpenInventoryOfActor(_subjectPawn);

    _inventoryWidgetCloseHandle = inventorySubsys->OnInventoryClosedFromUI().AddUObject(_subjectStateC.Get(), &UNewStateComponent::TryToggleInventory);

    // TODO: stop movement.
}

void UInventoryOpenState::OnLeave() {
    const auto inventorySubsys = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInventoryManipulationSubsystem>();
    inventorySubsys->CloseInventory();
    inventorySubsys->OnInventoryClosedFromUI().Remove(_inventoryWidgetCloseHandle);
}

TScriptInterface<IFSMState> UInventoryOpenState::Tick(const float deltaTime) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInventoryOpenState::TrySetMovementDestination(const FVector& movementDestination) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInventoryOpenState::TryStopMovement() {
    return _keepCurrentState();
}

FStateResponse_TryCastSkill UInventoryOpenState::TryCastSkillAtIndex(int32 index) {
    return FStateResponse_TryCastSkill{_keepCurrentState(), TOptional<FSkillCastResult>{}};
}

TScriptInterface<IFSMState> UInventoryOpenState::TryAbort() {
    return _keepCurrentState();
}

FStateResponse_TrySetSkillTarget UInventoryOpenState::TrySetSkillTarget(const FSkillTargetPacket& targetPacket) {
    return FStateResponse_TrySetSkillTarget{_keepCurrentState(), TOptional<TVariant<FSkillTargetingResult, FSkillCastResult>>{}};
}

TScriptInterface<IFSMState> UInventoryOpenState::TryInteracting() {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInventoryOpenState::TryToggleInventory() {
    TScriptInterface<IFSMState> skillExecutionState = NewObject<USkillExecutionState>(this, USkillExecutionState::StaticClass());
    skillExecutionState->SetPawn(_subjectPawn.Get());
    return MoveTemp(skillExecutionState);
}
