// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldCharacter.h"

#include "Colorizer.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "EnvironmentCell.h"
#include "EnvironmentGridWorldSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TemperatureDamageType.h"

AScalarFieldCharacter::AScalarFieldCharacter() {
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
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	_cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	_cameraBoom->SetupAttachment(RootComponent);
	_cameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	_cameraBoom->TargetArmLength = 800.f;
	_cameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	_cameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	_topDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	_topDownCameraComponent->SetupAttachment(_cameraBoom, USpringArmComponent::SocketName);
	_topDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	_thermodynamicC = CreateDefaultSubobject<UThermodynamicComponent>(TEXT("Thermodynamic Component"));
	_impactDmgHandlerC = CreateDefaultSubobject<UPawnImpactDamageHandlerComponent>(TEXT("Impact Damage Handler Component"));

	// Create a mana component...
	_manaC = CreateDefaultSubobject<UManaComponent>(TEXT("Mana Component"));

	// Create a health component...
	_healthC = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	// Create a skills container component...
	_skillsContainer = CreateDefaultSubobject<USkillsContainerComponent>(TEXT("Skills Container Component"));

	// Create the component that handles temperature damage...
	_temperatureDmgHandlerC = CreateDefaultSubobject<UTemperatureDamageHandlerComponent>(TEXT("Temperature Damage Handler Component"));

	_inventoryC = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));

	_factionC = CreateDefaultSubobject<UFactionComponent>(TEXT("Faction Component"));

	_stimuliSourceC = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimuli Source Component"));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

float AScalarFieldCharacter::TakeDamage(const float damageAmount, const FDamageEvent& damageEvent, AController* const eventInstigator, AActor* const damageCauser) {
	float damage = Super::TakeDamage(damageAmount, damageEvent, eventInstigator, damageCauser);

	check(IsValid(_healthC));

	// TODO: apply damage resistances here

	if (!_healthC->IsDead()) {
		_healthC->TakeDamage(damageAmount);
	}

	/*if (_healthC->IsDead()) {
		OnDeath();
	}*/

	return damage;
}

void AScalarFieldCharacter::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	_temperatureDmgHandlerC->HandleDamage(_thermodynamicC->GetTemperature());
}

void AScalarFieldCharacter::BeginPlay() {
	Super::BeginPlay();

	_setupThermodynamicCollisions();
	_thermodynamicC->OnTemperatureChanged.AddUObject(this, &AScalarFieldCharacter::_temperatureChanged);

	_dmiSetup();
	_setOverlappingCells();
}

void AScalarFieldCharacter::_setupThermodynamicCollisions() {
	const auto simpleThermalCollisions = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName{ "SimpleThermalCollision" });
	check(simpleThermalCollisions.Num() == 1);
	_simpleThermalCollision = Cast<UPrimitiveComponent>(simpleThermalCollisions[0]);

	const auto complexThermalCollisions = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName{ "ComplexThermalCollision" });
	check(complexThermalCollisions.Num() <= 1);

	if (!complexThermalCollisions.IsEmpty()) {
		_complexThermalCollision = Cast<UPrimitiveComponent>(complexThermalCollisions[0]);
	}
	_thermodynamicC->SetCollision(_simpleThermalCollision, _complexThermalCollision);
}

void AScalarFieldCharacter::_dmiSetup() {
	// Setting up the DMI that changes the mesh color based on temperature
	_materialInstance = GetMesh()->CreateDynamicMaterialInstance(0, GetMesh()->GetMaterial(0), TEXT("Thermodynamics Material"));

	if (_materialInstance != nullptr) {
		const FLinearColor temperatureColor = FColorizer::GenerateColorFromTemperature(_thermodynamicC->GetTemperature());
		_updateMaterialTint(temperatureColor);
	}
}

void AScalarFieldCharacter::_setOverlappingCells() {
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

void AScalarFieldCharacter::_updateMaterialTint(const FLinearColor temperatureColor) {
	check(_materialInstance != nullptr);
	_materialInstance->SetVectorParameterValue(TEXT("Tint"), temperatureColor);
}

void AScalarFieldCharacter::_temperatureChanged(double newTemperature) {
	const FLinearColor temperatureColor = FColorizer::GenerateColorFromTemperature(_thermodynamicC->GetTemperature());
	_updateMaterialTint(temperatureColor);
}

