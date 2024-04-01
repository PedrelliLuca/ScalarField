// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FactionComponent.h"

#include "Components/AIStatusWidgetComponent.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "ManaComponent.h"
#include "NewSkillsContainerComponent.h"
#include "NewStateComponent.h"
#include "PawnImpactDamageHandlerComponent.h"
#include "SkillsContainerComponent.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicComponent.h"

#include "EnemyMageCharacter.generated.h"

/*!
 * \brief Character structured for a top-view game.
 */
UCLASS(Blueprintable)
class AEnemyMageCharacter : public ACharacter {
    GENERATED_BODY()

public:
    AEnemyMageCharacter();

    float TakeDamage(float damageAmount, const FDamageEvent& damageEvent, AController* eventInstigator, AActor* damageCauser) override;
    void Tick(float deltaTime) override;

protected:
    void BeginPlay() override;

private:
    void _setupThermodynamicCollisions();
    void _dmiSetup();
    void _updateMaterialTint(FLinearColor temperatureColor);

    void _temperatureChanged(double newTemperature);

    void _die();
    void _onDeathMontageEnded(UAnimMontage* montage, bool bInterrupted);

    UPROPERTY()
    TObjectPtr<UPrimitiveComponent> _simpleThermalCollision = nullptr;

    UPROPERTY()
    TObjectPtr<UPrimitiveComponent> _complexThermalCollision = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Thermodynamics", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UThermodynamicComponent> _thermodynamicC;

    UPROPERTY(VisibleAnywhere, Category = Mana, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UManaComponent> _manaC;

    UPROPERTY(VisibleAnywhere, Category = Health)
    TObjectPtr<UHealthComponent> _healthC;

    UPROPERTY(VisibleAnywhere, Category = Skills, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USkillsContainerComponent> _skillsContainerC;

    UPROPERTY(VisibleAnywhere, Category = Skills, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNewSkillsContainerComponent> _newSkillsContainerC;

    UPROPERTY(VisibleAnywhere, Category = "State Machine", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UNewStateComponent> _stateC;

    UPROPERTY(VisibleAnywhere, Category = "Damage Handling", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UTemperatureDamageHandlerComponent> _temperatureDmgHandlerC;

    UPROPERTY(VisibleAnywhere, Category = "Damage Handling")
    TObjectPtr<UPawnImpactDamageHandlerComponent> _impactDmgHandlerC;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInventoryComponent> _inventoryC;

    UPROPERTY(VisibleAnywhere, Category = "Faction and Relationships")
    TObjectPtr<UFactionComponent> _factionC;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAIStatusWidgetComponent> _aiStatusWidgetC;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> _materialInstance;

    UPROPERTY(EditAnywhere, Category = "Animation")
    TObjectPtr<UAnimMontage> _deathMontage = nullptr;

    static constexpr uint32 KEY_ASSIGNABLE_SKILLS = 10;

    FOnMontageBlendingOutStarted _montageBlendingOutStartDelegate;
};
