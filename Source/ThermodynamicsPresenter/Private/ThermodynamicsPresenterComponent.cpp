// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermodynamicsPresenterComponent.h"

#include "Materials/MaterialParameterCollectionInstance.h"
#include "TemperatureColorConverter.h"
#include "ThermodynamicsInteractorComponent.h"
#include "ThermodynamicsSubsystem.h"

UThermodynamicsPresenterComponent::UThermodynamicsPresenterComponent() {
    PrimaryComponentTick.bCanEverTick = false;
    PrimaryComponentTick.TickGroup = ETickingGroup::TG_PostUpdateWork;
}

FLinearColor UThermodynamicsPresenterComponent::GetTemperatureColor() const {
    return _thermodynamicsColor;
}

void UThermodynamicsPresenterComponent::BeginPlay() {
    Super::BeginPlay();

    if (const auto thermoInteractorC = GetOwner()->FindComponentByClass<UThermodynamicsInteractorComponent>(); IsValid(thermoInteractorC)) {
        const TArray<UActorComponent*> possibleMeshes = GetOwner()->GetComponentsByTag(UMeshComponent::StaticClass(), FName{THERMODYNAMICS_MESH_TAG});

        if (!possibleMeshes.IsEmpty()) {
            check(possibleMeshes.Num() == 1);

            if (const auto thermodynamicsMesh = Cast<UMeshComponent>(possibleMeshes[0]); IsValid(thermodynamicsMesh)) {
                if (const auto material = thermodynamicsMesh->GetMaterial(_materialIndex); IsValid(material)) {
                    _thermodynamicsMaterialInstance = thermodynamicsMesh->CreateDynamicMaterialInstance(0, material, TEXT("Thermodynamics Material"));
                    check(IsValid(_thermodynamicsMaterialInstance));
                    thermoInteractorC->OnTemperatureChanged.AddUObject(this, &UThermodynamicsPresenterComponent::_updateThermodynamicsPresentation);

                    if (_toggleVisibilityMPC) {
                        const auto thermoSubsys = GetWorld()->GetSubsystem<UThermodynamicsSubsystem>();
                        thermoSubsys->OnHeatmapVisualizationToggle.AddUObject(this, &UThermodynamicsPresenterComponent::_toggleThermodynamicsPresentation);
                        _toggleVisibilityMPCI = GetWorld()->GetParameterCollectionInstance(_toggleVisibilityMPC);
                    } else {
                        UE_LOG(LogTemp, Error, TEXT("No UMaterialParameterCollection specified, temperature presentation cannot be toggled!"));
                    }
                } else {
                    UE_LOG(
                        LogTemp, Error, TEXT("Tag %s found on a non-UMeshComponent. No thermodynamics presentation on this actor!"), *FString(THERMODYNAMICS_MESH_TAG));
                }
            } else {
                UE_LOG(LogTemp, Error, TEXT("No component with tag %s was found. No thermodynamics presentation on this actor!"), *FString(THERMODYNAMICS_MESH_TAG));
            }
        } else {
            UE_LOG(LogTemp, Error, TEXT("No UThermodynamicsInteractorComponent was found. No thermodynamics presentation on this actor!"));
        }
    }
}

void UThermodynamicsPresenterComponent::_updateThermodynamicsPresentation(const float temperature) {
    check(IsValid(_thermodynamicsMaterialInstance));

    _thermodynamicsColor = FTemperatureColorConverter::TemperatureToColor(temperature);
    _thermodynamicsMaterialInstance->SetVectorParameterValue(_parameterName, _thermodynamicsColor);
}

void UThermodynamicsPresenterComponent::_toggleThermodynamicsPresentation() {
    check(_toggleVisibilityMPCI.IsValid());
    _isThermodynamicsVisible = !_isThermodynamicsVisible;

     float visibiltyValue = 0.0f;
     if (_isThermodynamicsVisible) {
         visibiltyValue = 1.0f;
     }

    _toggleVisibilityMPCI->SetScalarParameterValue(_toggleParameterName, visibiltyValue);
}
