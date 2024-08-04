// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "SkillIconContainerWidget.generated.h"

class UHorizontalBox;
class USkillsContainerComponent;
class USkillIconWidget;
class UWrapBox;

/**
 *
 */
UCLASS(Blueprintable)
class GAMEPLAYUI_API USkillIconContainerWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void BuildIconsFromContainer(const TObjectPtr<USkillsContainerComponent>& skillsContainer);

protected:
    UPROPERTY(EditDefaultsOnly, NoClear, Category = "Skill Icon Container Widget")
    TSubclassOf<USkillIconWidget> _skillIconClass;

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWrapBox> _skillSlots;
};
