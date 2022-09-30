// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThermodynamicComponent.h"

#include "ThermodynamicActor.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SCALARFIELD_API AThermodynamicActor : public AActor {
	GENERATED_BODY()
	
public:
	AThermodynamicActor();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Thermodynamics, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> _mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Thermodynamics, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UThermodynamicComponent> _thermodynamicC;
};
