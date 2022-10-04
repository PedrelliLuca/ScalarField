// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScalarFieldCharacter.h"

#include "Camera/CameraComponent.h"
#include "Colorizer.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "EnvironmentCell.h"
#include "EnvironmentGridWorldSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "ThermodynamicsSettings.h"
#include "UObject/ConstructorHelpers.h"

AScalarFieldCharacter::AScalarFieldCharacter()
{
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

	// Create a thermodynamic component...
	_thermodynamicC = CreateDefaultSubobject<UThermodynamicComponent>(TEXT("Thermodynamic Component"));
	_thermodynamicC->SetupAttachment(RootComponent);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AScalarFieldCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AScalarFieldCharacter::BeginPlay() {
	Super::BeginPlay();

	// Setting up the DMI that changes the mesh color based on temperature
	auto thermodynamicsSettings = GetDefault<UThermodynamicsSettings>();
	_materialInstance = GetMesh()->CreateDynamicMaterialInstance(0, GetMesh()->GetMaterial(0), TEXT("Thermodynamics Material"));

	if (_materialInstance != nullptr) {
		_updateMaterialBasedOnTemperature(_thermodynamicC->GetTemperature());
		_thermodynamicC->OnTemperatureChanged.AddUObject(this, &AScalarFieldCharacter::_updateMaterialBasedOnTemperature);
	}

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

void AScalarFieldCharacter::_updateMaterialBasedOnTemperature(double temperature) {
	check(!_materialInstance.IsNull())
	_materialInstance->SetVectorParameterValue(TEXT("Tint"), FColorizer::GenerateColorFromTemperature(temperature));
}