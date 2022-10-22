// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillUserState.h"
#include "NiagaraSystem.h"

#include "IdleState.generated.h"


/**
 * \brief State representing a skill user that is not executing any skill.
 */
UCLASS(Blueprintable)
class SKILLUSERFSM_API UIdleState : public USkillUserState {
	GENERATED_BODY()

public:
	TObjectPtr<USkillUserState> OnLMBPress(TObjectPtr<AController> controller) override;
	virtual TObjectPtr<USkillUserState> OnLMBRelease(TObjectPtr<APlayerController> controller) override;
	TObjectPtr<USkillUserState> OnKey1Press(TObjectPtr<AController> controller) override;

	void Tick(float deltaTime, TObjectPtr<APlayerController> controller) override;
	void OnEnter(TObjectPtr<AController> controller) override;
	void OnLeave(TObjectPtr<AController> controller) override;

protected:
	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UNiagaraSystem* _fxCursor;

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	double _shortPressThreshold = 1.;

private:
	bool _bInputPressed; // Input is being pressed
	double _followTime; // For how long it has been pressed
};
