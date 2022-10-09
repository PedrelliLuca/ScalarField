// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ManaComponent.h"
#include "Skills/AbstractSkill.h"
#include "ThermodynamicComponent.h"

#include "ScalarFieldCharacter.generated.h"

/*!
* \brief Character structured for a top-view game.
*/
UCLASS(Blueprintable)
class AScalarFieldCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AScalarFieldCharacter();

	void Tick(float DeltaSeconds) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return _topDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return _cameraBoom; }

	void CastSkillAtIndex(uint32 index);

protected:
	void BeginPlay() override;

private:
	void _dmiSetup();
	void _setOverlappingCells();
	void _updateMaterialBasedOnTemperature(double temperature);

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* _topDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* _cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Thermodynamics, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UThermodynamicComponent> _thermodynamicC;

	UPROPERTY(VisibleAnywhere, Category = Mana, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UManaComponent> _manaC;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> _materialInstance;

	UPROPERTY(EditAnywhere, Category = "Skills")
	TArray<FSkillParameters> _skillsParameters;

	UPROPERTY()
	TArray<TObjectPtr<UAbstractSkill>> _skills;

	static constexpr uint32 ASSIGNABLE_SKILLS = 10;
};

