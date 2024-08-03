// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "SkillIconContainerWidget.generated.h"

class USkillsContainerComponent;

/**
 *
 */
UCLASS(Blueprintable)
class GAMEPLAYUI_API USkillIconContainerWidget : public UUserWidget {
    GENERATED_BODY()

public:
    void BuildIconsFromContainer(TObjectPtr<USkillsContainerComponent> skillsContainer);
};
