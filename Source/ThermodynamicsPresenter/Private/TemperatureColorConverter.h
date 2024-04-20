// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FTemperatureColorConverter {
public:
    static FLinearColor TemperatureToColor(const float temperature) {
        if (temperature <= TEMPERATURE_BLUE) {
            return BLUE;
        } else if (temperature <= TEMPERATURE_CYAN) {
            return _interpolateInRGB(temperature, TEMPERATURE_BLUE, TEMPERATURE_CYAN, BLUE, CYAN);
        } else if (temperature <= TEMPERATURE_GREEN) {
            return _interpolateInRGB(temperature, TEMPERATURE_CYAN, TEMPERATURE_GREEN, CYAN, GREEN);
        } else if (temperature <= TEMPERATURE_YELLOW) {
            return _interpolateInRGB(temperature, TEMPERATURE_GREEN, TEMPERATURE_YELLOW, GREEN, YELLOW);
        } else if (temperature <= TEMPERATURE_RED) {
            return _interpolateInRGB(temperature, TEMPERATURE_YELLOW, TEMPERATURE_RED, YELLOW, RED);
        } else if (temperature <= TEMPERATURE_WINE_RED) {
            return _interpolateInRGB(temperature, TEMPERATURE_RED, TEMPERATURE_WINE_RED, RED, WINE_RED);
        }
        return WINE_RED;
    }

private:
    static FLinearColor _interpolateInRGB(const float value, const float min, const float max, const FLinearColor colorMin, const FLinearColor colorMax) {
        const float alphaValue = (value - min) / (max - min);
        return FMath::Lerp(colorMin, colorMax, alphaValue);
    }

    // TODO: Find a way to set these without having to re-compile.

    static constexpr float TEMPERATURE_BLUE = 73.f;       // Roughly -200 Celsius
    static constexpr float TEMPERATURE_CYAN = 273.f;      // Roughly 0 Celsius
    static constexpr float TEMPERATURE_GREEN = 303.f;     // Roughly 30 Celsius
    static constexpr float TEMPERATURE_YELLOW = 333.f;    // Roughly 60 Celsius
    static constexpr float TEMPERATURE_RED = 473.f;       // Roughly 200 Celsius
    static constexpr float TEMPERATURE_WINE_RED = 1273.f; // Roughly 1000 Celsius

    static constexpr FLinearColor BLUE{0.0f, 0.0f, 1.0f};
    static constexpr FLinearColor CYAN{0.0f, 1.0f, 1.0f};
    static constexpr FLinearColor GREEN{0.0f, 1.0f, 0.0f};
    static constexpr FLinearColor YELLOW{1.0f, 1.0f, 0.0f};
    static constexpr FLinearColor RED{1.0f, 0.0f, 0.0f};
    static constexpr FLinearColor WINE_RED{0.82f, 0.0f, 0.41f};
};
