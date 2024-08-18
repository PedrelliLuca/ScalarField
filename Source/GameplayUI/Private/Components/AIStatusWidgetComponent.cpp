// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/AIStatusWidgetComponent.h"

#include "PawnBindableWidgetInterface.h"

UAIStatusWidgetComponent::UAIStatusWidgetComponent() {
    Space = EWidgetSpace::Screen;
    DrawSize = FIntPoint(200, 50);
    bDrawAtDesiredSize = true;

    SetActive(true);
}

void UAIStatusWidgetComponent::BeginPlay() {
    Super::BeginPlay();

    if (APawn* ownerPawn = Cast<APawn>(GetOwner()); IsValid(ownerPawn)) {
        if (const auto aiStatusWidget = Cast<IPawnBindableWidget>(GetUserWidgetObject())) {
            // TODO: why not actor? See IPawnBindableWidget comment.
            aiStatusWidget->SetPawn(ownerPawn);
        } else {
            UE_LOG(LogTemp, Error, TEXT("%s: Invalid User Widget, must be an AITemperatureWidget"), *FString{__FUNCTION__});
        }
    } else {
        // TODO: and why would this be a problem? Ugh...
        UE_LOG(LogTemp, Error, TEXT("%s: Owner is not an APawn."), *FString{__FUNCTION__});
    }
}
