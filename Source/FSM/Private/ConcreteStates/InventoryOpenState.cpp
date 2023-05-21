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

TScriptInterface<IFSMState> UInventoryOpenState::TryCastSkillAtIndex(int32 index) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInventoryOpenState::TryAbortSkillInExecution() {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInventoryOpenState::TrySetSkillTarget(const FSkillTargetPacket& targetPacket) {
    return _keepCurrentState();
}

TScriptInterface<IFSMState> UInventoryOpenState::TryToggleInventory() {
    TScriptInterface<IFSMState> skillExecutionState = NewObject<USkillExecutionState>(this, USkillExecutionState::StaticClass());
    skillExecutionState->SetPawn(_subjectPawn.Get());
    return MoveTemp(skillExecutionState);
}
