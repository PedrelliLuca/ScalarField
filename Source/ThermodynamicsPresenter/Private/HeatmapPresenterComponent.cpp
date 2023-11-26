// Fill out your copyright notice in the Description page of Project Settings.

#include "HeatmapPresenterComponent.h"

#include "GameFramework/Actor.h"
#include "ThermodynamicsSubsystem.h"

UHeatmapPresenterComponent::UHeatmapPresenterComponent() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UHeatmapPresenterComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    const float frequency = _frequencyPI * PI;
    const float phase = _phasePI * PI;
    const float sinWaveValue = _offset + _amplitude * FMath::Sin(phase + frequency * _timer);
    const float cosWaveValue = _offset + _amplitude * FMath::Cos(phase + frequency * _timer);
    const FColor sinColor = _generateColorFromValue(sinWaveValue);
    const FColor cosColor = _generateColorFromValue(cosWaveValue);

    FTexture2DMipMap* const mipMap = &_heatmapTexture->PlatformData->Mips[0];
    FByteBulkData* const bulkData = &mipMap->BulkData;
    uint8* const rawImageData = static_cast<uint8*>(bulkData->Lock(LOCK_READ_WRITE));

    // [Pixel1 B][Pixel1 G][Pixel1 R][Pixel1 A][Pixel2 B][Pixel2 G][Pixel2 R][Pixel2 A]
    const int32 nPixels = _numCellsX * _numCellsY;
    constexpr int32 STEP = 4;
    constexpr int32 BLUE_OFFSET = 0;
    constexpr int32 GREEN_OFFSET = 1;
    constexpr int32 RED_OFFSET = 2;
    constexpr int32 ALPHA_OFFSET = 3;
    for (int32 i = 0; i < nPixels; ++i) {
        const bool isOdd = i & 1;
        const FColor pixelColor = isOdd ? cosColor : sinColor;

        const int32 pixelStart = STEP * i;
        rawImageData[pixelStart + BLUE_OFFSET] = static_cast<uint8>(pixelColor.B);
        rawImageData[pixelStart + GREEN_OFFSET] = static_cast<uint8>(pixelColor.G);
        rawImageData[pixelStart + RED_OFFSET] = static_cast<uint8>(pixelColor.R);
        rawImageData[pixelStart + ALPHA_OFFSET] = static_cast<uint8>(pixelColor.A);
    }

    bulkData->Unlock();
    _heatmapTexture->UpdateResource();

    _timer += deltaTime;
}

void UHeatmapPresenterComponent::BeginPlay() {
    Super::BeginPlay();

    _heatmapMID = UMaterialInstanceDynamic::Create(_heatmapMaterial, this, FName(""));

    _heatmapTexture = UTexture2D::CreateTransient(_numCellsX, _numCellsY);
    _heatmapTexture->Filter = TextureFilter::TF_Nearest;

    _heatmapMID->SetTextureParameterValue(FName("GridTexture"), _heatmapTexture);

    UThermodynamicsSubsystem* thermoSubsys = GetWorld()->GetSubsystem<UThermodynamicsSubsystem>();
    thermoSubsys->SetHeatmapMaterialInstance(_heatmapMID);
}

FColor UHeatmapPresenterComponent::_generateColorFromValue(float val) {
    const float intervalMax = _offset + _amplitude;
    const float intervalMin = _offset - _amplitude;

    const float oneQuarter = intervalMin + 0.25f * (intervalMax - intervalMin);
    const float half = intervalMin + 0.5f * (intervalMax - intervalMin);
    const float threeQuarters = intervalMin + 0.75f * (intervalMax - intervalMin);

    FColor outColor;

    if (val < intervalMin || val > intervalMax) {
        outColor = FColor::Black;
    } else {
        FColor maxColor;
        FColor minColor;
        float subMax;
        float subMin;
        if (val <= oneQuarter) {
            subMax = oneQuarter;
            subMin = intervalMin;
            maxColor = FColor::Cyan;
            minColor = FColor::Blue;
        } else if (val <= half) {
            subMax = half;
            subMin = oneQuarter;
            maxColor = FColor::Green;
            minColor = FColor::Cyan;
        } else if (val <= threeQuarters) {
            subMax = threeQuarters;
            subMin = half;
            maxColor = FColor::Yellow;
            minColor = FColor::Green;
        } else {
            subMax = intervalMax;
            subMin = threeQuarters;
            maxColor = FColor::Red;
            minColor = FColor::Yellow;
        }

        const float alpha = (val - subMin) / (subMax - subMin);

        outColor = FMath::Lerp(minColor.ReinterpretAsLinear(), maxColor.ReinterpretAsLinear(), alpha).ToFColor(false);
    }

    return outColor;
}
