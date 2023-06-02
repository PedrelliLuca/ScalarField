// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCastResult.h"
#include "SkillTargetingResult.h"

class IFSMState;

/** This header collects all structs representing a response to some IFSMState function call. */

/** \brief The return type of IFSMState::TryCastSkill(). This encapsulates the potential new state to move to, as well as the result of the skill cast. */
struct FStateResponse_TryCastSkill {
    TScriptInterface<IFSMState> StateResponse;
    TOptional<FSkillCastResult> CastResponse;
};

/** \brief The return type of IFSMState::TrySetSkillTarget(). This encapsulates the potential new state to move to, as well as the result of the skill
 * targeting. */
struct FStateResponse_TrySetSkillTarget {
    TScriptInterface<IFSMState> StateResponse;
    TOptional<FSkillTargetingResult> TargetingResponse;
};