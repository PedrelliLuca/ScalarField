// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryOpenState.h"

void UInventoryOpenState::SetPawn(TObjectPtr<APawn> subjectPawn) {
}

void UInventoryOpenState::OnEnter() {
}

void UInventoryOpenState::OnLeave() {
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
    return _keepCurrentState();
}
