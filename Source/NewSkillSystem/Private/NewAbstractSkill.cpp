// Fill out your copyright notice in the Description page of Project Settings.

#include "NewAbstractSkill.h"

FSkillCastResult UNewAbstractSkill::TryCast() {
    return FSkillCastResult::CastFail_Cooldown();
}
