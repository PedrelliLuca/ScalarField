// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

void UHUDWidget::NativeConstruct() {
	Super::NativeConstruct();
}

void UHUDWidget::SetMaxHealth(double newMaxHealth) {
	_maxHealth = newMaxHealth;
	SetHealth();
}

void UHUDWidget::SetCurrentHealth(double newCurrentHealth) {
	_currentHealth = newCurrentHealth;
	SetHealth();
}

void UHUDWidget::SetMaxMana(double newMaxMana) {
	_maxMana = newMaxMana;
	SetMana();
}

void UHUDWidget::SetCurrentMana(double newCurrentMana) {
	_currentMana = newCurrentMana;
	SetMana();
}
