// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvironmentCell.h"

#include "EnvironmentGridSettings.h"

AEnvironmentCell::AEnvironmentCell()
{
	PrimaryActorTick.bCanEverTick = false;

	_boxC = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(_boxC);

	_side = GetDefault<UEnvironmentGridSettings>()->GetCellSide();
}

