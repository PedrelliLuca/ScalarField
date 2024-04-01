// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "DeathAnimationComponent.generated.h"

class ACharacter;
class UAnimMontage;

/**
 * \brief TODO
 */
UCLASS(ClassGroup = (Animation), meta = (BlueprintSpawnableComponent))
class ANIMATIONCOMPONENTS_API UDeathAnimationComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UDeathAnimationComponent();

protected:
    void BeginPlay() override;

private:
    void _playDeathMontage(TObjectPtr<AActor> _);
    void _onDeathMontageEnded(UAnimMontage* montage, bool bInterrupted) const;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UAnimMontage> _deathMontage;

    FOnMontageBlendingOutStarted _montageBlendingOutStartDelegate;

    TWeakObjectPtr<ACharacter> _ownerCharacter;
};
