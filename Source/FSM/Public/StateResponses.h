// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillCastResult.h"

struct FStateResponse_TryCastSkill {
    TScriptInterface<class IFSMState> StateResponse;
    TOptional<FSkillCastResult> CastResponse;
};
