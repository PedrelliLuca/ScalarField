// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "ManaComponent.h"
#include "SkillsContainerComponent.h"
#include "TemperatureDamageHandlerComponent.h"
#include "NewThermodynamicComponent.h"

#include "NewScalarFieldCharacter.generated.h"

/*!
* \brief Character structured for a top-view game.
*/
UCLASS(Blueprintable)
class ANewScalarFieldCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ANewScalarFieldCharacter();

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return _topDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return _cameraBoom; }

	float TakeDamage(float damageAmount, const FDamageEvent& damageEvent, AController* eventInstigator, AActor* damageCauser) override;
	void Tick(float deltaTime) override;

protected:
	void BeginPlay() override;

private:
	void _dmiSetup();
	void _setOverlappingCells();
	void _updateMaterialTint(FLinearColor temperatureColor);

	void _temperatureChanged(double newTemperature);

	void _healthChanged(double newHealth) const;
	void _maxHealthChanged(double newMaxHealth) const;
	void _healthRegenChanged(double newHealthRegen) const;

	void _manaChanged(double newHealth) const;
	void _maxManaChanged(double newMaxHealth) const;
	void _manaRegenChanged(double newHealthRegen) const;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* _topDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* _cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Thermodynamics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> _thermodynamicCapsuleC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Thermodynamics", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNewThermodynamicComponent> _thermodynamicC;

	UPROPERTY(VisibleAnywhere, Category = Mana, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UManaComponent> _manaC;

	UPROPERTY(VisibleAnywhere, Category = Health)
	TObjectPtr<UHealthComponent> _healthC;

	UPROPERTY(VisibleAnywhere, Category = Skills, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkillsContainerComponent> _skillsContainer;

	UPROPERTY(VisibleAnywhere, Category = "Damage Handling", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTemperatureDamageHandlerComponent> _temperatureDmgHandlerC;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _materialInstance;

	static constexpr uint32 KEY_ASSIGNABLE_SKILLS = 10;
};

