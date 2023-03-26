// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "MaterialsContainerComponent.generated.h"

UENUM()
enum class EMaterial : uint8
{
    M_Carbon = 0 UMETA(DisplayName = "Carbon"),
    M_Water = 1 UMETA(DisplayName = "Water"),
    M_Dirt = 2 UMETA(DisplayName = "Dirt"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MATERIALSYSTEM_API UMaterialsContainerComponent : public UActorComponent {
    GENERATED_BODY()

public:
    bool HasMaterial(EMaterial material) const { return _materialsToQuantities.Contains(material); }

private:
    UPROPERTY(EditAnywhere, Category = "Materials")
    TMap<EMaterial, double> _materialsToQuantities;
};
