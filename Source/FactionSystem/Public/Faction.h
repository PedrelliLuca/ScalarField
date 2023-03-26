// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Faction.generated.h"

UENUM(BlueprintType)
enum class EFaction : uint8
{
    Factionless UMETA(FisplayName = "Factionless"),
    GoodGuys UMETA(DisplayName = "Good Guys"),
    BadGuys UMETA(DisplayName = "Bad Guys")
};
