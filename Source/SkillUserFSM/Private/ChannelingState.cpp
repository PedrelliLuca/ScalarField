// Fill out your copyright notice in the Description page of Project Settings.


#include "ChannelingState.h"



TObjectPtr<USkillUserState> UChannelingState::OnTargeting(TObjectPtr<AActor> target, TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnBeginSkillExecution(const int32 skillKey, TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnTick(float deltaTime, TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

TObjectPtr<USkillUserState> UChannelingState::OnSkillExecutionAborted(TObjectPtr<AController> controller) {
	return _keepCurrentState();
}

void UChannelingState::OnEnter(TObjectPtr<AController> controller) {
}

void UChannelingState::OnLeave(TObjectPtr<AController> controller) {
}
