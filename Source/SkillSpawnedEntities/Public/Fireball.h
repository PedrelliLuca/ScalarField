// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SkillSpawnedEntityInterface.h"

#include "Fireball.generated.h"

class UThermodynamicsInteractorComponent;

UCLASS()
class SKILLSPAWNEDENTITIES_API AFireball : public AActor, public ISkillSpawnedEntity {
    GENERATED_BODY()

public:
    AFireball();

    void Tick(float deltaTime) override;
    void SetCaster(TWeakObjectPtr<AActor> caster) override { _caster = caster; }

protected:
    void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    TObjectPtr<UThermodynamicsInteractorComponent> _thermoC = nullptr;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UStaticMeshComponent> _staticMeshC = nullptr;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UProjectileMovementComponent> _projectileMovementC = nullptr;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UParticleSystemComponent> _particleSystem = nullptr;

    UPROPERTY(EditAnywhere, Category = "Fireball | Particles")
    TObjectPtr<UParticleSystem> _fireballTemplate;

    // If the fireball temperature reaches a value below this threshold, it destroys itself
    UPROPERTY(EditAnywhere, Category = "Fireball | Thresholds")
    float _temperatureExtinctionThreshold = 270.0f;

    UPROPERTY(EditAnywhere, Category = "Fireball | Thresholds")
    float _distanceExtinctionThreshold = 10000.0f;

    TWeakObjectPtr<AActor> _caster = nullptr;
};
