// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidget.h"

void UInteractionWidget::UpdateInteractionWidget(const UInteractableComponent* interactionComponent) {
	check(interactionComponent != nullptr);
	_owningInteractionComponent = interactionComponent;
	_onUpdateInteractionWidget();
}
