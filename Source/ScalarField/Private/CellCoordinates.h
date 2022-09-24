// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"

struct FCellCoordinates {
public:
	bool operator==(const FCellCoordinates& other) const {
		return (X == other.X) && (Y == other.Y);
	}

	int32 X;
	int32 Y;
};

#if UE_BUILD_DEBUG

uint32 GetTypeHash(const FCellCoordinates& cellCoords);

#else // optimize by inlining in shipping and development builds

FORCEINLINE uint32 GetTypeHash(const FCellCoordinates& cellCoords) {
	return HashCombine(GetTypeHash(cellCoords.X), GetTypeHash(cellCoords.Y));
}

#endif