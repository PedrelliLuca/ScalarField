// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EMovementCommandMode : uint8 {
	MCM_Still = 0				UMETA(DisplayName = "Still"),
	MCM_Rotation = 1			UMETA(DisplayName = "Rotation"),
	MCM_Translation = 2			UMETA(DisplayName = "Translation"),
	MCM_RotoTranslation = 3		UMETA(DisplayName = "Roto-Translation"),
};
