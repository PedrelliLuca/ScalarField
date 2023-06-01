// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/** \brief The logic of the Player Movement Command might depend on how the input was provided. This struct stores the information on the Player's input to be
 * provided to the command. */
struct FPlayerInputData {
    EInputEvent SetDestinationInputEvent{};
};
