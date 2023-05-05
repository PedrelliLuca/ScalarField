// Fill out your copyright notice in the Description page of Project Settings.

#include "NewChannelingState.h"

void UNewChannelingState::SetPawn(TObjectPtr<APawn> subjectPawn) {
}

void UNewChannelingState::OnEnter() {
}

void UNewChannelingState::OnLeave() {
}

TScriptInterface<IFSMState> UNewChannelingState::TryCastSkillAtIndex(int32 index) {
    return _keepCurrentState();
}
