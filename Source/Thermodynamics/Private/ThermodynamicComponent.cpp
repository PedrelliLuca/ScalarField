// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermodynamicComponent.h"

UThermodynamicComponent::UThermodynamicComponent(const FObjectInitializer& ObjectInitializer) {
    PrimaryComponentTick.bCanEverTick = true;

    _currentTemperature = _initialTemperature;
    _nextTemperature = _initialTemperature;
}

void UThermodynamicComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    if (_bFirstTick) {
        _bFirstTick = false;
        return;
    }

    if (_bCollisionChangedSinceLastTick) {
        if (GetOwner()->GetActorLabel() == FString{"BP_ConeOfCold0"}) {
            UE_LOG(LogTemp, Warning, TEXT("Set initial exchangers call"));
        }
        // UPrimitiveComponent::OnComponentBeginOverlap does not fire before the first tick. We need this call to get the collisions we're already
        // overlapping with.
        _setInitialExchangers();
        _bCollisionChangedSinceLastTick = false;
    }

    // We'll get checked for each potential heat exchanger.
    _timesToBeCheckedThisFrame = _possibleHeatExchangers.Num();

    if (const auto ownerPawn = Cast<const APawn>(GetOwner())) {
        if (auto playerController = Cast<const APlayerController>(ownerPawn->GetController())) {
            if (_timesToBeCheckedThisFrame < _counterOfChecksThisFrame) {
                UE_LOG(LogTemp, Warning, TEXT("Something"));
            }
        }
    }

    _nextTemperature = _currentTemperature + _getTemperatureDelta(deltaTime);

    if (_counterOfChecksThisFrame == _timesToBeCheckedThisFrame) {
        if (const auto ownerPawn = Cast<const APawn>(GetOwner())) {
            if (auto playerController = Cast<const APlayerController>(ownerPawn->GetController())) {
                UE_LOG(LogTemp, Warning, TEXT("FROM SELF:"));
            }
        }
        _setCurrentTempAsNext();
    }
}

#if WITH_EDITOR
void UThermodynamicComponent::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) {
    FProperty* const property = propertyChangedEvent.Property;
    FName propertyName = property != nullptr ? property->GetFName() : NAME_None;
    if (propertyName == GET_MEMBER_NAME_CHECKED(UThermodynamicComponent, _initialTemperature)) {
        if (const auto initTempProperty = CastFieldChecked<FDoubleProperty>(property)) {
            SetTemperature(initTempProperty->GetFloatingPointPropertyValue(property->ContainerPtrToValuePtr<double>(this)), true);
        }
    }

    Super::PostEditChangeProperty(propertyChangedEvent);
}
#endif

void UThermodynamicComponent::SetTemperature(double temperature, const bool updateInitialTemp /*= false*/) {
    temperature = FMath::Clamp(temperature, 0., TNumericLimits<double>::Max());

    if (updateInitialTemp) {
        // Updating _initialTemperature just to give a visual feedback in the editor, this value doesn't matter during play
        _initialTemperature = temperature;
    }

    _currentTemperature = temperature;
    _nextTemperature = temperature;
}

void UThermodynamicComponent::SetCollision(TObjectPtr<UPrimitiveComponent> simpleCollision, TObjectPtr<UPrimitiveComponent> complexCollision /*= nullptr*/) {
    check(IsValid(simpleCollision));

    if (_simpleCollisionC.IsValid()) {
        _simpleCollisionC->OnComponentBeginOverlap.RemoveDynamic(this, &UThermodynamicComponent::_onSimpleBeginOverlap);
        _simpleCollisionC->OnComponentEndOverlap.RemoveDynamic(this, &UThermodynamicComponent::_onSimpleEndOverlap);

        _counterOfChecksThisFrame = 0;
        _timesToBeCheckedThisFrame = TNumericLimits<uint32>::Max();
        _possibleHeatExchangers.Empty();
    }

    // Is the input collision an actual thermodynamic collider? And can it generate overlap events?
    check(simpleCollision->GetCollisionProfileName() == TEXT("HeatExchanger"));
    check(simpleCollision->GetGenerateOverlapEvents());

    _simpleCollisionC = simpleCollision;
    _simpleCollisionC->OnComponentBeginOverlap.AddDynamic(this, &UThermodynamicComponent::_onSimpleBeginOverlap);
    _simpleCollisionC->OnComponentEndOverlap.AddDynamic(this, &UThermodynamicComponent::_onSimpleEndOverlap);

    _complexCollisionC = nullptr;
    if (IsValid(complexCollision)) {
        // Is the input collision an actual thermodynamic collider?
        if (complexCollision->GetCollisionProfileName() != TEXT("NoCollision")) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): Complex Collision profile name not set to \"No Collision\", it will be forced!"), *FString{__FUNCTION__});
            complexCollision->SetCollisionProfileName(TEXT("NoCollision"));
        }

        // I decided to not check() on the bGenerateOverlapEvents of the complex, since skeletal meshes are a classic example of meshes where this value
        // is set to false by default and are valid complex meshes
        if (!complexCollision->GetGenerateOverlapEvents()) {
            UE_LOG(LogTemp, Warning, TEXT("%s(): Complex Collision set to unable to generate overlap events, it will be forced!"), *FString{__FUNCTION__});
            complexCollision->SetGenerateOverlapEvents(true);
        }

        _complexCollisionC = complexCollision;
    }

    _bCollisionChangedSinceLastTick = true;
}

void UThermodynamicComponent::BeginPlay() {
    Super::BeginPlay();
    SetTemperature(_initialTemperature);
}

double UThermodynamicComponent::_getTemperatureDelta(float deltaTime) {
    double deltaTemperature = 0.0;
    if (_possibleHeatExchangers.Num() == 0) {
        return deltaTemperature;
    }

    const auto thisCollision = _getMostComplexCollision();

    int32 nActualHeatExchangers = 0;
    // Here this component performs the heat-checks on the other components.
    for (const auto& otherThermoC : _possibleHeatExchangers) {
        check(otherThermoC.IsValid());
        const auto otherCollison = otherThermoC->_getMostComplexCollision();

        /* If the following evaluates to true, that means that otherThermoC is an actual heatExchanger for thisThermoC.
         * CAUTION: if this returns false even though you're sure an overlap is in place, that's because one of the two
         * UPrimitiveComponent::bGenerateOverlapEvents attributes is set to false. This can easily happen with skeletal
         * meshes on characters for example.*/
        if (thisCollision->IsOverlappingComponent(otherCollison.Get())) {
            ++nActualHeatExchangers;

            /* When this is hotter than other, the delta is negative since we emit heat.
             * When this is colder than other, the delta is positive since we absorb heat. */
            deltaTemperature += (otherThermoC->_currentTemperature - _currentTemperature);
        }

        if (const auto otherPawn = Cast<const APawn>(otherThermoC->GetOwner())) {
            if (auto otherPlayerController = Cast<const APlayerController>(otherPawn->GetController())) {
                UE_LOG(LogTemp, Warning, TEXT("%s is increasing player counter"), *(GetOwner()->GetActorLabel()));
                if (GetOwner()->GetActorLabel() == FString{"BP_ConeOfCold0"}) {
                    UE_LOG(LogTemp, Warning, TEXT("YOU FUCKING BITCH!"))
                }
            }
        }

        // We heat-checked otherThermoC, so it must increase its counter
        otherThermoC->_updateCounterOfChecksThisFrame();
    }

    if (nActualHeatExchangers == 0) {
        return deltaTemperature;
    }

    deltaTemperature /= nActualHeatExchangers;
    deltaTemperature *= ROD_CONSTANT * deltaTime / _heatCapacity;
    return deltaTemperature;
}

void UThermodynamicComponent::_updateCounterOfChecksThisFrame() {
    ++_counterOfChecksThisFrame;
    if (_counterOfChecksThisFrame == _timesToBeCheckedThisFrame) {
        if (const auto ownerPawn = Cast<const APawn>(GetOwner())) {
            if (auto playerController = Cast<const APlayerController>(ownerPawn->GetController())) {
                UE_LOG(LogTemp, Warning, TEXT("FROM OTHER:"));
            }
        }

        _setCurrentTempAsNext();
    }
}

void UThermodynamicComponent::_setCurrentTempAsNext() {
    _currentTemperature = _nextTemperature;
    OnTemperatureChanged.Broadcast(_currentTemperature);

    _counterOfChecksThisFrame = 0;
    _timesToBeCheckedThisFrame = TNumericLimits<uint32>::Max();

    if (const auto ownerPawn = Cast<const APawn>(GetOwner())) {
        if (auto playerController = Cast<const APlayerController>(ownerPawn->GetController())) {
            UE_LOG(LogTemp, Warning, TEXT("--------- RESET ---------"));
        }
    }
}

void UThermodynamicComponent::_setInitialExchangers() {
    check(_simpleCollisionC.IsValid());

    TArray<TObjectPtr<UPrimitiveComponent>> overlappingComponents;
    _simpleCollisionC->GetOverlappingComponents(overlappingComponents);

    for (const auto& otherC : overlappingComponents) {
        const auto otherOwner = otherC->GetOwner();

        const auto otherThermoC = otherOwner->FindComponentByClass<UThermodynamicComponent>();
        // If I have a thermodynamic collision I must have a thermodynamic component
        check(IsValid(otherThermoC));

        // Filtering out every collision that's not simple VS simple
        if (otherThermoC->_simpleCollisionC.Get() != otherC) {
            check(otherThermoC->_complexCollisionC.Get() == otherC);
            continue;
        }

        _possibleHeatExchangers.Emplace(otherThermoC);
    }

    if (_complexCollisionC.IsValid() && _possibleHeatExchangers.Num() > 0) {
        // We have at least one possible heat excvhanger, wake up the complex collision!
        _complexCollisionC->SetCollisionProfileName(TEXT("HeatExchanger"));
    }
}

void UThermodynamicComponent::_onSimpleBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp,
    int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {
    const auto otherThermoC = otherActor->FindComponentByClass<UThermodynamicComponent>();
    check(IsValid(otherThermoC));

    if (const auto pawn = Cast<const APawn>(GetOwner())) {
        if (auto otherPlayerController = Cast<const APlayerController>(pawn->GetController())) {
            if (otherActor->GetActorLabel() == FString{"BP_ConeOfCold0"}) {
                UE_LOG(LogTemp, Warning, TEXT("COLLISION!"))
            }
        }
    }

    // Filtering out every collision that's not simple VS simple
    if (otherThermoC->_simpleCollisionC.Get() != otherComp) {
        // NOTE: THIS WHOLE SYSTEM WORKS AS LONG AS YOU HAVE A SINGLE THERMODYNAMIC COMPONENT FOR EACH ACTOR. Only if this hypothesys holds you can be sure
        // otherComp is either the otherThermoC's simple or complex collision.
        check(otherThermoC->_complexCollisionC.Get() == otherComp);
        return;
    }

    _possibleHeatExchangers.Emplace(otherThermoC);

    if (_complexCollisionC.IsValid() && _complexCollisionC->GetCollisionProfileName() != FName{TEXT("HeatExchanger")}) {
        // We have at least one possible heat excvhanger, activate the complex collision!
        _complexCollisionC->SetCollisionProfileName(TEXT("HeatExchanger"));
    }
}

void UThermodynamicComponent::_onSimpleEndOverlap(
    UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex) {
    const auto thermoCToRemove = Algo::FindByPredicate(_possibleHeatExchangers,
        [&otherComp](const TWeakObjectPtr<UThermodynamicComponent>& thermoC) { return thermoC->_simpleCollisionC.Get() == otherComp; });

    // thermoCToRemove could be nullptr, meaning that otherComp is a complex collision
    if (thermoCToRemove != nullptr) {
        _possibleHeatExchangers.Remove(*thermoCToRemove);

        if (_complexCollisionC.IsValid() && _possibleHeatExchangers.Num() == 0) {
            // No more possible heat exchangers, disable the complex collision.
            _complexCollisionC->SetCollisionProfileName(TEXT("NoCollision"));
        }
    }
}

TWeakObjectPtr<UPrimitiveComponent> UThermodynamicComponent::_getMostComplexCollision() {
    if (_complexCollisionC.IsValid()) {
        return _complexCollisionC;
    }

    return _simpleCollisionC;
}
