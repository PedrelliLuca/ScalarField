// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EMovementCommandMode : uint8 {
	MCM_None = 0                UMETA(DisplayName = "None"),
	MCM_Still = 1				UMETA(DisplayName = "Still"),
	MCM_Rotation = 2			UMETA(DisplayName = "Rotation"),
	MCM_Translation = 3			UMETA(DisplayName = "Translation"),
	MCM_RotoTranslation = 4		UMETA(DisplayName = "Roto-Translation"),
};
