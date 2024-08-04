// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "SkillIconContainerWidget.generated.h"

class USkillsContainerComponent;
class UWrapBox;

/**
 *
 */
UCLASS(Blueprintable)
class GAMEPLAYUI_API USkillIconContainerWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void BuildIconsFromContainer(const TObjectPtr<USkillsContainerComponent>& skillsContainer);

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWrapBox> _skillSlots;
};
