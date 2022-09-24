// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"

struct FCellCoordinates {
public:
	bool operator==(const FCellCoordinates& other) const {
		return FMath::IsNearlyEqual(X, other.X) && FMath::IsNearlyEqual(Y, other.Y);
	}

	double X;
	double Y;
};

#if UE_BUILD_DEBUG

uint32 GetTypeHash(const FCellCoordinates& cellCoords);

#else // optimize by inlining in shipping and development builds

FORCEINLINE uint32 GetTypeHash(const FCellCoordinates& cellCoords) {
	return HashCombine(GetTypeHash(cellCoords.X), GetTypeHash(cellCoords.Y));
}

#endif