// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/*!
 * \brief Struct that stores the integer coordinates of a cell in 2D space.
 */
struct FCellCoordinates {
public:
    /*! \brief This is needed for the struct to be hashable. */
    bool operator==(const FCellCoordinates& other) const { return (X == other.X) && (Y == other.Y); }

    bool operator!=(const FCellCoordinates& other) const { return !(*this == other); }

    operator FVector2D() const { return FVector2D{static_cast<double>(X), static_cast<double>(Y)}; }

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

/*! \brief This is needed for FCellCoordinates to be hashable. */
FORCEINLINE uint32 GetTypeHash(const FCellCoordinates& cellCoords) {
    return HashCombine(GetTypeHash(cellCoords.X), GetTypeHash(cellCoords.Y));
}

#endif
