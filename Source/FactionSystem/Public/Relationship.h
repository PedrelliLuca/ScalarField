// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Relationship.generated.h"

UENUM(BlueprintType)
enum class ERelationship : uint8
{
    Neutral UMETA(DisplayName = "Neutral"),
    Ally UMETA(DisplayName = "Ally"),
    Enemy UMETA(DisplayName = "Enemy")
};
