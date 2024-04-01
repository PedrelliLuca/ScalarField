// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnemyMageCharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Colorizer.h"
#include "Components/CapsuleComponent.h"
#include "EnvironmentCell.h"
#include "EnvironmentGridWorldSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InventoryManipulationSubsystem.h"
#include "Kismet/GameplayStatics.h"

AEnemyMageCharacter::AEnemyMageCharacter() {
    // This is what makes the scalar field character interact with the environment grid
    GetCapsuleComponent()->SetCollisionProfileName("GridInteractingPawn");

    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Creation of the widget component that shows this character's temperature
    _aiStatusWidgetC = CreateDefaultSubobject<UAIStatusWidgetComponent>(TEXT("AI Temperature Widget Component"));
    _aiStatusWidgetC->SetupAttachment(RootComponent);

    _thermodynamicC = CreateDefaultSubobject<UThermodynamicComponent>(TEXT("Thermodynamic Component"));

    // Create a mana component...
    _manaC = CreateDefaultSubobject<UManaComponent>(TEXT("Mana Component"));

    // Create a health component...
    _healthC = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

    // Create a skills container component...
    _skillsContainerC = CreateDefaultSubobject<USkillsContainerComponent>(TEXT("Skills Container Component"));
    _newSkillsContainerC = CreateDefaultSubobject<UNewSkillsContainerComponent>(TEXT("NewSkillsContainerComponent"));

    _stateC = CreateDefaultSubobject<UNewStateComponent>(TEXT("StateComponent"));

    // Create the component that handles temperature damage...
    _temperatureDmgHandlerC = CreateDefaultSubobject<UTemperatureDamageHandlerComponent>(TEXT("Temperature Damage Handler Component"));
    _impactDmgHandlerC = CreateDefaultSubobject<UPawnImpactDamageHandlerComponent>(TEXT("Impact Damage Handler Component"));

    _factionC = CreateDefaultSubobject<UFactionComponent>(TEXT("Faction Component"));

    _inventoryC = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

float AEnemyMageCharacter::TakeDamage(
    const float damageAmount, const FDamageEvent& damageEvent, AController* const eventInstigator, AActor* const damageCauser) {
    // Dead things can't take any more damage
    if (_healthC->IsDead()) {
        return 0.0f;
    }

    const float damage = Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);

    // TODO: apply damage resistances here

    _healthC->TakeDamage(damageAmount);

    if (_healthC->IsDead()) {
        _die();
    }

    return damage;
}

void AEnemyMageCharacter::Tick(float deltaTime) {
    Super::Tick(deltaTime);

    // _temperatureDmgHandlerC->HandleDamage(_thermodynamicC->GetTemperature());
}

void AEnemyMageCharacter::BeginPlay() {
    Super::BeginPlay();

    _setupThermodynamicCollisions();
    _thermodynamicC->OnTemperatureChanged.AddUObject(this, &AEnemyMageCharacter::_temperatureChanged);

    _dmiSetup();
    _setOverlappingCells();

    const auto inventorySubsys = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UInventoryManipulationSubsystem>();
    check(IsValid(inventorySubsys));
    inventorySubsys->SetupDeathDropForActor(this);
}

void AEnemyMageCharacter::_setupThermodynamicCollisions() {
    const auto simpleThermalCollisions = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName{"SimpleThermalCollision"});
    check(simpleThermalCollisions.Num() == 1);
    _simpleThermalCollision = Cast<UPrimitiveComponent>(simpleThermalCollisions[0]);

    const auto complexThermalCollisions = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName{"ComplexThermalCollision"});
    check(complexThermalCollisions.Num() <= 1);

    if (!complexThermalCollisions.IsEmpty()) {
        _complexThermalCollision = Cast<UPrimitiveComponent>(complexThermalCollisions[0]);
    }
    _thermodynamicC->SetCollision(_simpleThermalCollision, _complexThermalCollision);
}

void AEnemyMageCharacter::_dmiSetup() {
    // Setting up the DMI that changes the mesh color based on temperature
    _materialInstance = GetMesh()->CreateDynamicMaterialInstance(0, GetMesh()->GetMaterial(0), TEXT("Thermodynamics Material"));

    if (_materialInstance != nullptr) {
        const FLinearColor temperatureColor = FColorizer::GenerateColorFromTemperature(_thermodynamicC->GetTemperature());
        _updateMaterialTint(temperatureColor);
    }
}

void AEnemyMageCharacter::_setOverlappingCells() {
    // Retrieve all environment cells being overlapped at startup
    TSet<AActor*> overlappingActors;
    GetCapsuleComponent()->GetOverlappingActors(overlappingActors, AEnvironmentCell::StaticClass());

    TSet<AEnvironmentCell*> overlappingCells;
    Algo::Transform(overlappingActors, overlappingCells, [](AActor* const overlappingActor) {
        AEnvironmentCell* overlappingCell = Cast<AEnvironmentCell>(overlappingActor);
        check(overlappingCell != nullptr);
        return overlappingCell;
    });

    // Send the cells to the grid subsystem to unfreeze them
    GetWorld()->GetSubsystem<UEnvironmentGridWorldSubsystem>()->ActivateOverlappedCells(overlappingCells);
}

void AEnemyMageCharacter::_updateMaterialTint(const FLinearColor temperatureColor) {
    check(_materialInstance != nullptr);
    _materialInstance->SetVectorParameterValue(TEXT("Tint"), temperatureColor);
}

void AEnemyMageCharacter::_temperatureChanged(double newTemperature) {
    const FLinearColor temperatureColor = FColorizer::GenerateColorFromTemperature(_thermodynamicC->GetTemperature());
    _updateMaterialTint(temperatureColor);
}

void AEnemyMageCharacter::_die() {
    const auto aiController = GetController<AAIController>();
    check(IsValid(aiController));

    _healthC->SetHealthRegen(0.0);
    _manaC->SetManaRegen(0.0);

    GetMovementComponent()->StopMovementImmediately();

    const auto reason = FString{TEXT("Death")};
    aiController->GetBrainComponent()->StopLogic(reason);

    constexpr float playRate = 1.0f;
    const bool playedSuccessfully = PlayAnimMontage(_deathMontage, playRate) > 0.0f;
    if (playedSuccessfully) {
        const auto animInstance = GetMesh()->GetAnimInstance();
        if (!_montageBlendingOutStartDelegate.IsBound()) {
            _montageBlendingOutStartDelegate.BindUObject(this, &AEnemyMageCharacter::_onDeathMontageEnded);
        }

        // Montage_SetEndDelegate() is not good, because the end of the delegate is way after its blend out time. In other words, you'd see the character going
        // back to the idle animation.
        animInstance->Montage_SetBlendingOutDelegate(_montageBlendingOutStartDelegate, _deathMontage);
    }
}

void AEnemyMageCharacter::_onDeathMontageEnded(UAnimMontage* const montage, const bool bInterrupted) {
    check(montage == _deathMontage);
    Destroy();
}
