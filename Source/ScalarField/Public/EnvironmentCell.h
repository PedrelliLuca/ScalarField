// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"

#include "EnvironmentCell.generated.h"

class UBoxComponent;

UCLASS()
class SCALARFIELD_API AEnvironmentCell : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnvironmentCell();

	TWeakObjectPtr<UBoxComponent> GetBox() { return _boxC; }
	double GetSide() const { return _side; }

private:
	double _side;

	TObjectPtr<UBoxComponent> _boxC;
};

