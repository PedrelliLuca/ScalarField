// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCastResult.h"

/** This header collects all structs representing a response to some IFSMState function call. */

/** \brief The return type of IFSMState::TryCastSkill(). This encapsulates the potential new state to move to, as well as the result of the skill cast. */
struct FStateResponse_TryCastSkill {
    TScriptInterface<class IFSMState> StateResponse;
    TOptional<FSkillCastResult> CastResponse;
};
