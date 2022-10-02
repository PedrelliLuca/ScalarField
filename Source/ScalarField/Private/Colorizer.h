// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FColorizer {
public:
	static FLinearColor GenerateColorFromTemperature(const double temperature) {
		if (temperature <= 75.) {
			return BLUE;
		}
		else if (temperature <= 283.) {
			return _interpolateInRGB(temperature, 73., 283., BLUE, CYAN);
		}
		else if (temperature <= 303.) {
			return _interpolateInRGB(temperature, 283, 303., CYAN, YELLOW);
		}
		else if (temperature <= 323.) {
			return _interpolateInRGB(temperature, 303, 323., YELLOW, ORANGE);
		}
		else if (temperature <= 473.) {
			return _interpolateInRGB(temperature, 323, 473., ORANGE, RED);
		}
		else if (temperature <= 1273.) {
			return _interpolateInRGB(temperature, 473, 1273., RED, WINE_RED);
		}

		return WINE_RED;
	}

private:
	static FLinearColor _interpolateInRGB(const double value, const double min, const double max, const FLinearColor colorMin, const FLinearColor colorMax) {
		const double alphaValue = (value - min) / (max - min);
		return FMath::Lerp(colorMin, colorMax, alphaValue);
	}

	static constexpr FLinearColor WINE_RED{ 0.82f, 0.f, 0.41f };
	static constexpr FLinearColor RED{ 1.f, 0.f, 0.f };
	static constexpr FLinearColor ORANGE{ 1.f, 0.27f, 0.f };
	static constexpr FLinearColor YELLOW{ 1.f, 1.f, 0.f };
	static constexpr FLinearColor CYAN{ 0.f, 1.f, 1.f };
	static constexpr FLinearColor BLUE{ 0.f, 0.f, 1.f };
};
