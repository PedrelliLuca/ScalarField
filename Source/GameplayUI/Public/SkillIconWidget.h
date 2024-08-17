// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "SkillIconWidget.generated.h"

class FSkillPropertiesInspector;
class UImage;

/**
 *
 */
UCLASS(Blueprintable)
class GAMEPLAYUI_API USkillIconWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void InitFromSkillProperties(const FSkillPropertiesInspector& skillProp);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void _onSkillStatusChanged(ESkillStatus newStatus);

    UFUNCTION(BlueprintImplementableEvent)
    void _onCooldownUpdate(float remainingSeconds, bool firstUpdate);

    UPROPERTY(EditDefaultsOnly, Category = "Skill Icon Widget", meta = (ClampMin = "0"))
    float _intervalBetweenCooldownUpdates = 0.1f;

private:
    void _setupCooldownTimer(ESkillStatus newStatus);

    float _skillCooldownSeconds;

    FTimerHandle _cooldownTimer;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> _skillThumbnail;
};
