// Fill out your copyright notice in the Description page of Project Settings.

#include "TemperatureDamageHandlerComponent.h"

#include "HealthComponent.h"
#include "ThermodynamicsInteractorComponent.h"

UTemperatureDamageHandlerComponent::UTemperatureDamageHandlerComponent() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UTemperatureDamageHandlerComponent::TickComponent(float const deltaTime, ELevelTick const tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    // TickComponent() manages the cooldown of the temperature damage deltaTime
    if (_isTemperatureCausingDamage()) {
        SetComponentTickEnabled(false);

        FTimerDelegate delegate;
        delegate.BindLambda([weakThis = TWeakObjectPtr<UTemperatureDamageHandlerComponent>(this), healthC = _healthC]() {
            // Do not re-enable the tick if the owner is already dead.
            if (weakThis.IsValid() && healthC.IsValid() && !healthC->IsDead()) {
                weakThis->SetComponentTickEnabled(true);
            }
        });

        GetWorld()->GetTimerManager().SetTimer(_damageCooldownTimerHandle, delegate, _damageInterval, false);
    }
}

void UTemperatureDamageHandlerComponent::SetMinComfortTemperature(double const minComfortTemperature) {
    _minComfortTemperature = minComfortTemperature > 0. ? minComfortTemperature : 0.;
}

void UTemperatureDamageHandlerComponent::SetMaxComfortTemperature(double const maxComfortTemperature) {
    _maxComfortTemperature = maxComfortTemperature > _minComfortTemperature ? maxComfortTemperature : _minComfortTemperature + DBL_EPSILON;
}

#if WITH_EDITOR
void UTemperatureDamageHandlerComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
    FProperty* const property = propertyChangedEvent.Property;
    FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
    if (propertyName == GET_MEMBER_NAME_CHECKED(UTemperatureDamageHandlerComponent, _minComfortTemperature)) {
        if (auto const initTempProperty = CastFieldChecked<FDoubleProperty>(property)) {
            SetMinComfortTemperature(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<double>(this)));
        }
    } else if (propertyName == GET_MEMBER_NAME_CHECKED(UTemperatureDamageHandlerComponent, _maxComfortTemperature)) {
        if (auto const initTempProperty = CastFieldChecked<FDoubleProperty>(property)) {
            SetMaxComfortTemperature(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<double>(this)));
        }
    }

    Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UTemperatureDamageHandlerComponent::BeginPlay() {
    Super::BeginPlay();

    bool isInitializationSuccessfull = true;

    _healthC = GetOwner()->FindComponentByClass<UHealthComponent>();
    if (!_healthC.IsValid()) {
        UE_LOG(LogTemp, Warning, TEXT("%s: Owner does not have a valid UHealthComponent and won't receive damage by temperature"), *FString(__FUNCTION__));
        isInitializationSuccessfull = false;
    }

    _thermoInteractorC = GetOwner()->FindComponentByClass<UThermodynamicsInteractorComponent>();
    if (!_thermoInteractorC.IsValid()) {
        UE_LOG(LogTemp, Warning, TEXT("%s: Owner does not have a valid UThermodynamicsInteractorComponent and won't receive damage by temperature"),
            *FString(__FUNCTION__));
        isInitializationSuccessfull = false;
    }

    if (!isInitializationSuccessfull) {
        SetComponentTickEnabled(false);
    }
}

bool UTemperatureDamageHandlerComponent::_isTemperatureCausingDamage() const {
    bool isTemperatureCausingDamage = false;

    check(_thermoInteractorC.IsValid());
    float const temperature = _thermoInteractorC->GetTemperature();

    bool const isTemperatureComfortable = temperature >= _minComfortTemperature && temperature <= _maxComfortTemperature;
    if (!isTemperatureComfortable) {
        check(_healthC.IsValid());

        // TODO: apply damage resistances here. Even if temperature is uncomfortable, the damage might still be zero!

        _healthC->TakeDamage(_computeDamageForTemperature(temperature));
        isTemperatureCausingDamage = true;
    }
    return isTemperatureCausingDamage;
}

float UTemperatureDamageHandlerComponent::_computeDamageForTemperature(float const temperature) const {
    if (temperature < _minComfortTemperature) {
        return _minComfortTemperature - temperature;
    } else if (temperature > _maxComfortTemperature) {
        return temperature - _maxComfortTemperature;
    }

    checkNoEntry();
    return 0.0f;
}
