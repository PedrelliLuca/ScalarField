// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "ThermodynamicsPresenterComponent.generated.h"

/**
 * \brief TODO
 */
UCLASS(ClassGroup = (Thermodynamics), meta = (BlueprintSpawnableComponent))
class THERMODYNAMICSPRESENTER_API UThermodynamicsPresenterComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UThermodynamicsPresenterComponent();

    FLinearColor GetTemperatureColor() const;

protected:
    void BeginPlay() override;

private:
    void _updateThermodynamicsPresentation(float temperature);

    void _toggleThermodynamicsPresentation();

    UPROPERTY(EditAnywhere, Category = "Presentation")
    int32 _materialIndex;

    UPROPERTY(EditAnywhere, Category = "Presentation")
    FName _parameterName;

    UPROPERTY(EditAnywhere, Category = "Material Parameter Collection")
    TObjectPtr<UMaterialParameterCollection> _toggleVisibilityMPC;

    UPROPERTY(EditAnywhere, Category = "Material Parameter Collection")
    FName _toggleParameterName = "";

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> _thermodynamicsMaterialInstance;

    TWeakObjectPtr<UMaterialParameterCollectionInstance> _toggleVisibilityMPCI;
    bool _isThermodynamicsVisible = true;

    FLinearColor _thermodynamicsColor = FLinearColor::Black;

    // TODO: this should be read via UDeveloperSettings
    inline static char const* THERMODYNAMICS_MESH_TAG = "ThermodynamicsMesh";
};
