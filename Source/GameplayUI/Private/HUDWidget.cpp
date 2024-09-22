// Fill out your copyright notice in the Description page of Project Settings.

#include "HUDWidget.h"

#include "GameFramework/Character.h"
#include "GameplayAttributesWidget.h"
#include "HealthComponent.h"
#include "SkillIconContainerWidget.h"
#include "TacticalPauseWorldSubsystem.h"
#include "ThermometerWidget.h"

void UHUDWidget::SetPawn(const TWeakObjectPtr<APawn> pawn) {
    ForgetCurrentPawn();

    _healthC = pawn->FindComponentByClass<UHealthComponent>();
    if (!_healthC.IsValid()) {
        UE_LOG(LogTemp, Error, TEXT(__FUNCTION__ ": missing UHealthComponent"));
    }

    _gameplayAttributesWidget->SetPawn(pawn);
    _thermometerWidget->SetPawn(pawn);
}

void UHUDWidget::ForgetCurrentPawn() {
    _unbindAll();
    _gameplayAttributesWidget->ForgetCurrentPawn();
    _thermometerWidget->ForgetCurrentPawn();
}

void UHUDWidget::Show() {
    _gameplayAttributesWidget->Show();
    _thermometerWidget->Show();
    _bindAll();
    AddToViewport();
}

void UHUDWidget::Hide() {
    RemoveFromParent();
    _unbindAll();
    _gameplayAttributesWidget->Hide();
    _thermometerWidget->Hide();
}

void UHUDWidget::InitSkillIconContainer(const TObjectPtr<USkillsContainerComponent>& skillsContainer, const TObjectPtr<UNewStateComponent>& stateMachine) {
    _skillIconContainer->BuildIconsFromContainer(skillsContainer, stateMachine);
}

void UHUDWidget::_bindAll() {
    if (_healthC.IsValid()) {
        _onDeathHandle = _healthC->OnDeath().AddUObject(this, &UHUDWidget::_onDeath);
    }

    check(!_pauseToggleHandle.IsValid());
    const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
    _setPauseStatus(pauseSubsys->IsTacticalPauseOn());
    _pauseToggleHandle = pauseSubsys->OnTacticalPauseToggle().AddUObject(this, &UHUDWidget::_onTacticalPauseToggle);
}

void UHUDWidget::_unbindAll() {
    if (_healthC.IsValid()) {
        _healthC->OnDeath().Remove(_onDeathHandle);
    }

    if (_pauseToggleHandle.IsValid()) {
        const auto pauseSubsys = GetWorld()->GetSubsystem<UTacticalPauseWorldSubsystem>();
        pauseSubsys->OnTacticalPauseToggle().Remove(_pauseToggleHandle);
        _pauseToggleHandle.Reset();
    }
}

// The parameter of this function is useless. In fact, this entire funciton shouldn't exist, UnbindCurrentPawn() should be the function bound to
// UHealthComponent::OnDeath(). However, due to an architectural error (see description of FOnDeath macro, UHealthComponent.h), it is necessary. I shall
// remove this one day.
void UHUDWidget::_onDeath(TObjectPtr<AActor> _) {
    ForgetCurrentPawn();
}

void UHUDWidget::_onTacticalPauseToggle(const bool bIsTacticalPauseOn, double) {
    _setPauseStatus(bIsTacticalPauseOn);
}
