// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeatMap.h"

AHeatMap::AHeatMap() {
    _staticMeshC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
    SetRootComponent(_staticMeshC);

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void AHeatMap::Tick(const float deltaTime) {
    Super::Tick(deltaTime);

    if (_materialInstance.IsValid()) {
        const float frequency = _frequencyPI * PI;
        const float phase = _phasePI * PI;
        const float topWaveValue = _offset + _amplitude * FMath::Sin(phase + frequency * _timer);
        _materialInstance->SetScalarParameterValue(FName("TopAlpha"), topWaveValue);

        const float bottomWaveValue = _offset + _amplitude * FMath::Cos(phase + frequency * _timer);
        _materialInstance->SetScalarParameterValue(FName("BottomAlpha"), bottomWaveValue);

        _timer += deltaTime;
    }
}

void AHeatMap::BeginPlay() {
    Super::BeginPlay();

    _materialInstance = _staticMeshC->CreateDynamicMaterialInstance(0, _material, FName(""));
    _staticMeshC->SetMaterial(0, _materialInstance.Get());

    /*const FBox boundingBox = _staticMeshC->GetStaticMesh()->GetBoundingBox();
    const FVector scale = _staticMeshC->GetRelativeTransform().GetScale3D();
    const int32 _width = static_cast<int32>(boundingBox.Max.X - boundingBox.Min.X) * scale.X;
    const int32 _height = static_cast<int32>(boundingBox.Max.Y - boundingBox.Min.Y) * scale.Y;
    const int32 nPixels = _width * _height;*/

    const int32 nPixels = _width * _height;
    const int32 firstHalf = nPixels / 2;

    // This texture has B = 1 for the first half. We don't give a shit about the other values, this is just to mask the color for a given region.
    UTexture2D* const topTexture = UTexture2D::CreateTransient(_width, _height);
    FTexture2DMipMap* const topMipMap = &topTexture->PlatformData->Mips[0];
    FByteBulkData* const topImageData = &topMipMap->BulkData;
    const int64 topSize = topImageData->GetBulkDataSize();

    // RawImageData is formatted as such:
    // [Pixel1 B][Pixel1 G][Pixel1 R][Pixel1 A][Pixel2 B][Pixel2 G][Pixel2 R][Pixel2 A] …
    uint8* const topRawImageData = static_cast<uint8*>(topImageData->Lock(LOCK_READ_WRITE));
    const int32 step = 4;
    const int32 alphaOffset = 3;
    for (int32 i = 0; i < firstHalf; ++i) {
        topRawImageData[i * step] = 255u;
        topRawImageData[i * step + alphaOffset] = 255u;
    }
    topImageData->Unlock();
    topTexture->UpdateResource();
    _materialInstance->SetTextureParameterValue(FName("TopTexture"), topTexture);

    // This texture has B = 1 for the second half. We don't give a shit about the other values, this is just to mask the color for a given region.
    UTexture2D* const bottomTexture = UTexture2D::CreateTransient(_width, _height);
    FTexture2DMipMap* const bottomMipMap = &bottomTexture->PlatformData->Mips[0];
    FByteBulkData* const bottomImageData = &bottomMipMap->BulkData;
    const int64 bottomSize = bottomImageData->GetBulkDataSize();
    // RawImageData is formatted as such:
    // [Pixel1 B][Pixel1 G][Pixel1 R][Pixel1 A][Pixel2 B][Pixel2 G][Pixel2 R][Pixel2 A] …
    uint8* const bottomRawImageData = static_cast<uint8*>(bottomImageData->Lock(LOCK_READ_WRITE));
    for (int32 i = firstHalf; i < nPixels; ++i) {
        bottomRawImageData[i * step] = 255u;
        bottomRawImageData[i * step + alphaOffset] = 255u;
    }
    bottomImageData->Unlock();
    bottomTexture->UpdateResource();
    _materialInstance->SetTextureParameterValue(FName("BottomTexture"), bottomTexture);
}
