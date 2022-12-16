// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPresenterWidget.h"


void UInventoryPresenterWidget::_onClose() {
	_onButtonClose.Broadcast();
}
