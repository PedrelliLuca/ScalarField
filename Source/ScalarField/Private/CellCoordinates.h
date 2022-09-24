// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"

/*!
* \brief A simple struct that stores the coordinates of a cell in 2D space. The coordinates are int32s to make hashing deterministic.
*/
struct FCellCoordinates {
public:
	bool operator==(const FCellCoordinates& other) const {
		return (X == other.X) && (Y == other.Y);
	}

	bool operator!=(const FCellCoordinates& other) const {
		return !(*this == other);
	}

	operator FVector2D() const {
		return FVector2D{ static_cast<double>(X), static_cast<double>(Y)};
	}

	FString ToString() const {
		FString coordsString = "(" + FString::FromInt(X) + ", " + FString::FromInt(Y) + ")";
		return coordsString;
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