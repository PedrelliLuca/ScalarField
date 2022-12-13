// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPresenterWidget.h"

#include "StateComponent.h"


void UInventoryPresenterWidget::OnClose() {
	const TWeakObjectPtr<UStateComponent> stateC = GetOwningPlayer()->FindComponentByClass<UStateComponent>();
	check(stateC.IsValid());
	check(stateC->IsLookingAtWidget());

	stateC->PerformInventoryToggleBehavior();
}
