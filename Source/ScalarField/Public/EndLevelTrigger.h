// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EndLevelTrigger.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class SCALARFIELD_API AEndLevelTrigger : public ATriggerBox {
    GENERATED_BODY()

protected:
    void BeginPlay() override;

private:
    UFUNCTION()
    void _onPlayerOverlap(AActor* overlappedActor, AActor* otherActor);
};
