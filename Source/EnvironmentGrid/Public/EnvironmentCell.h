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

/*!
* \brief Actor representing a cell partitioning the environment (level). These cells are used to manage the ticking of actors in ScalarField.
* ScalarField aims at simulating a realistic environment for the player, which implies having many actors ticking in the scene. These cells'
* purpose is to make sure that the game runs smoothly by making the environment away from the player, i.e. actors whose interaction with the
* player is negligible, stop ticking.
*/
UCLASS()
class ENVIRONMENTGRID_API AEnvironmentCell : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnvironmentCell();

	/*!
	* \brief Freezes this cell in time, making it stop ticking.
	*/
	void FreezeTime();

	/*!
	* \brief Unfreezes this cell in time, making its ticking resume. The cell immediately makes up for all the frames it lost.
	*/
	void UnfreezeTime();

	/*!
	* \brief Returns wheter or not this cell is ticking
	*/
	bool IsFrozen() const { return _isFrozen; }

	void SetSide(double side) { _boxC->SetBoxExtent(FVector::OneVector * side * 0.5); }
	double GetSide() const { return _boxC->GetUnscaledBoxExtent().X * 2; }

	void SetCoordinates(FCellCoordinates coordinates) { _coordinates = coordinates; }
	const FCellCoordinates& GetCoordinates() const { return _coordinates; }

	/*!
	* \brief Broadcasts when the cell starts being overlapped by the player character
	*/
	FOnCellBeginningOverlap OnCellBeginningOverlap;

	/*!
	* \brief Broadcasts when the cell stops being overlapped by the player character
	*/
	FOnCellEndingOverlap OnCellEndingOverlap;

private:
	UFUNCTION()
	void _onCellBeginningOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void _onCellEndingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool _isFrozen;
	UPROPERTY()
	TObjectPtr<UBoxComponent> _boxC;
	FCellCoordinates _coordinates;

	/*! \brief Moment FreezeTime() was called */
	FDateTime _freezingTime;

	// I am not sure if these are meant to stay at the moment, but they're useful for debugging purposes
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> _staticMeshC;
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _materialInstance;
};

