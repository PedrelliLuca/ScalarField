// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicLevelScript.h"

#include "Components/BoxComponent.h"
#include "EnvironmentCell.h"
#include "EngineUtils.h"

void AThermodynamicLevelScript::BeginPlay() {
	Super::BeginPlay();

	// Search for the special trigger box actor that determines how the environment grid and the air are spawned
	TObjectPtr<ATriggerBox> gridTrigger = nullptr;
	for (TActorIterator<ATriggerBox> actorIt(GetWorld()); actorIt; ++actorIt) {
		if ((*actorIt)->GetActorLabel() == "GridTriggerBox") {
			gridTrigger = *actorIt;
			break;
		}
	}

	if (gridTrigger == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("AThermodynamicLevelScript::BeginPlay() couldn't find ATriggerBox labeled \"GridTriggerBox\", environment grid won't be spawned."));
		return;
	}

	const auto environmentGridSubsys = GetWorld()->GetSubsystem<UEnvironmentGridWorldSubsystem>();
	check(environmentGridSubsys != nullptr);

	const auto gridAttributes = _buildGridSpawnAttributes(gridTrigger);
	environmentGridSubsys->SpawnGrid(gridAttributes);

	_generateAir(gridAttributes.Step);

	gridTrigger->Destroy();
}

UEnvironmentGridWorldSubsystem::FGridSpawnAttributes AThermodynamicLevelScript::_buildGridSpawnAttributes(TObjectPtr<ATriggerBox> gridTriggerBox) {
	UEnvironmentGridWorldSubsystem::FGridSpawnAttributes gridAttributes{};
	gridAttributes.GridCenter = FVector2D{ gridTriggerBox->GetActorLocation() };

	auto gridBox = CastChecked<UBoxComponent>(gridTriggerBox->GetCollisionComponent());
	const FVector boxExtent = gridBox->GetUnscaledBoxExtent();
	gridAttributes.ZExtension = gridTriggerBox->GetActorLocation().Z;

	// The step can't be greater than any of the dimensions of the box
	const FVector boxDimensions = boxExtent * 2;
	gridAttributes.Step = _gridStep;
	if (gridAttributes.Step > FMath::Min(boxDimensions.X, boxDimensions.Y)) {
		gridAttributes.Step = FMath::Min(boxDimensions.X, boxDimensions.Y);
		UE_LOG(LogTemp, Warning, TEXT("Grid Step changed from %f to %f because it was greater than one of the GridTriggerBox dimensions"), _gridStep, gridAttributes.Step);
	}

	gridAttributes.NCellsX = FMath::RoundToInt(boxDimensions.X / gridAttributes.Step);
	gridAttributes.NCellsY = FMath::RoundToInt(boxDimensions.Y / gridAttributes.Step);

	return gridAttributes;
}

void AThermodynamicLevelScript::_generateAir(const double cellSide) {
	const auto environmentGridSubsys = GetWorld()->GetSubsystem<UEnvironmentGridWorldSubsystem>();

	const double moleculeSide = cellSide / _moleculesPerCellSide;

	// For each cell...
	for (const auto& coordCellPair : environmentGridSubsys->GetGrid()) {
		check(coordCellPair.Value.IsValid());
		const FVector cellLocation = coordCellPair.Value.Get()->GetActorLocation();
		const FVector firstMoleculeLocation = cellLocation + FVector{ cellSide, -cellSide, 0. } *0.5 + FVector{ -moleculeSide, moleculeSide, 0. } *0.5;
		for (int i = 0; i > -_moleculesPerCellSide; --i) {
			for (int j = 0; j < _moleculesPerCellSide; ++j) {
				const FVector moleculeLocation = firstMoleculeLocation + FVector{ static_cast<double>(i), static_cast<double>(j), 0. } *moleculeSide;
				const FTransform moleculeTransform{ moleculeLocation };

				const TObjectPtr<AThermodynamicActor> molecule = GetWorld()->SpawnActor<AThermodynamicActor>(_moleculeClass, moleculeTransform);
				molecule->SetTemperature(_airTemperature);
				molecule->SetHeatCapacity(_airHeatCapacity);
				molecule->SetThermicCapsuleDimensions((moleculeSide + 2) * 0.5, (moleculeSide + 2) * 0.5);
			}
		}
	}
}
