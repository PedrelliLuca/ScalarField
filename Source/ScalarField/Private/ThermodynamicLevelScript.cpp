// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermodynamicLevelScript.h"

#include "Components/BoxComponent.h"
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

	environmentGridSubsys->SpawnGrid(_buildGridSpawnAttributes(gridTrigger));

	gridTrigger->Destroy();
}

UEnvironmentGridWorldSubsystem::FGridSpawnAttributes AThermodynamicLevelScript::_buildGridSpawnAttributes(TObjectPtr<ATriggerBox> gridTriggerBox) {
	UEnvironmentGridWorldSubsystem::FGridSpawnAttributes gridAttributes{};
	gridAttributes.GridCenter = FVector2D{ gridTriggerBox->GetActorLocation() };

	auto gridBox = CastChecked<UBoxComponent>(gridTriggerBox->GetCollisionComponent());
	const FVector boxDimensions = gridBox->GetUnscaledBoxExtent() * 2;

	// The step can't be greater than any of the dimensions of the box
	gridAttributes.Step = _gridStep;
	if (gridAttributes.Step > FMath::Min(boxDimensions.X, boxDimensions.Y)) {
		gridAttributes.Step = FMath::Min(boxDimensions.X, boxDimensions.Y);
		UE_LOG(LogTemp, Warning, TEXT("Grid Step changed from %f to %f because it was greater than one of the GridTriggerBox dimensions"), _gridStep, gridAttributes.Step);
	}

	gridAttributes.NCellsX = FMath::RoundToInt(boxDimensions.X / gridAttributes.Step);
	gridAttributes.NCellsY = FMath::RoundToInt(boxDimensions.Y / gridAttributes.Step);

	return gridAttributes;
}
