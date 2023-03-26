// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "InteractableComponent.h"

#include "InteractionWidget.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API UInteractionWidget : public UUserWidget {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void UpdateInteractionWidget(const UInteractableComponent* interactionComponent);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    void _onUpdateInteractionWidget();

    UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta = (ExposeOnSpawn))
    TWeakObjectPtr<const UInteractableComponent> _owningInteractionComponent;
};
