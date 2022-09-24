// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CellCoordinates.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

#include "EnvironmentCell.generated.h"

class UBoxComponent;
class UMaterialInstanceDynamic;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCellBeginningOverlap, FCellCoordinates);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCellEndingOverlap, FCellCoordinates);

UCLASS()
class SCALARFIELD_API AEnvironmentCell : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnvironmentCell();

	void SetCoordinates(FCellCoordinates coordinates) { _coordinates = coordinates; }

	void FreezeTime();
	void UnfreezeTime();
	bool IsFrozen() const { return _isFrozen; }

	TWeakObjectPtr<UBoxComponent> GetBox() { return _boxC; }
	double GetSide() const { return _side; }

	FOnCellBeginningOverlap OnCellBeginningOverlap;
	FOnCellEndingOverlap OnCellEndingOverlap;

private:
	UFUNCTION()
	void _onCellBeginningOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void _onCellEndingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	double _side;
	TObjectPtr<UBoxComponent> _boxC;
	FCellCoordinates _coordinates;
	bool _isFrozen;

	// Placeholder stuff
	TObjectPtr<UStaticMeshComponent> _staticMeshC;
	TObjectPtr<UMaterialInstanceDynamic> _materialInstance;
};

