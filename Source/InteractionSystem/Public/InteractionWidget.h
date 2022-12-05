// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionComponent.h"
#include "Blueprint/UserWidget.h"

#include "InteractionWidget.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API UInteractionWidget : public UUserWidget {
     GENERATED_BODY()
     
public:
     UFUNCTION(BlueprintCallable, Category = "Interaction")
     void UpdateInteractionWidget(const UInteractionComponent* interactionComponent);

protected:
     UFUNCTION(BlueprintImplementableEvent)
     void _onUpdateInteractionWidget();

     UPROPERTY(BlueprintReadOnly, Category = "Interaction", meta = (ExposeOnSpawn))
     TWeakObjectPtr<const UInteractionComponent> _owningInteractionComponent;
};
