// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FactionComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "ManaComponent.h"
#include "PawnImpactDamageHandlerComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "SkillsContainerComponent.h"
#include "TemperatureDamageHandlerComponent.h"
#include "ThermodynamicComponent.h"

#include "ScalarFieldCharacter.generated.h"

/*!
 * \brief Character structured for a top-view game.
 */
UCLASS(Blueprintable)
class AScalarFieldCharacter : public ACharacter {
    GENERATED_BODY()

public:
    AScalarFieldCharacter();

    float TakeDamage(float damageAmount, const FDamageEvent& damageEvent, AController* eventInstigator, AActor* damageCauser) override;
    void Tick(float deltaTime) override;

protected:
    void BeginPlay() override;

private:
    void _setupThermodynamicCollisions();
    void _dmiSetup();
    void _setOverlappingCells();
    void _updateMaterialTint(FLinearColor temperatureColor);

    void _temperatureChanged(double newTemperature);

    void _die();
    void _onDeathMontageEnded(UAnimMontage* montage, bool bInterrupted);

    /** Top down camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* _topDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* _cameraBoom;

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
    TObjectPtr<USkillsContainerComponent> _skillsContainer;

    UPROPERTY(VisibleAnywhere, Category = "Damage Handling", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UTemperatureDamageHandlerComponent> _temperatureDmgHandlerC;

    UPROPERTY(VisibleAnywhere, Category = "Damage Handling")
    TObjectPtr<UPawnImpactDamageHandlerComponent> _impactDmgHandlerC;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInventoryComponent> _inventoryC;

    UPROPERTY(VisibleAnywhere, Category = "Faction and Relationships")
    TObjectPtr<UFactionComponent> _factionC;

    UPROPERTY(VisibleAnywhere, Category = "AI Senses")
    TObjectPtr<UAIPerceptionStimuliSourceComponent> _stimuliSourceC;

    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> _materialInstance;

    UPROPERTY(EditAnywhere, Category = "Animation")
    TObjectPtr<UAnimMontage> _deathMontage = nullptr;

    static constexpr uint32 KEY_ASSIGNABLE_SKILLS = 10;

    FOnMontageEnded _montageEndedDelegate;
};
