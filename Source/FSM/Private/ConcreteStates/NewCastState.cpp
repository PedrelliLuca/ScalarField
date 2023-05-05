// Fill out your copyright notice in the Description page of Project Settings.

#include "NewCastState.h"

void UNewCastState::SetPawn(TObjectPtr<APawn> subjectPawn) {
}

void UNewCastState::OnEnter() {
}

void UNewCastState::OnLeave() {
}

TScriptInterface<IFSMState> UNewCastState::TryCastSkillAtIndex(int32 index) {
    return _keepCurrentState();
}
