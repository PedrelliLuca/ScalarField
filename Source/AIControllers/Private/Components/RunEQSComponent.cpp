// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/RunEQSComponent.h"

#include "AIController.h"

URunEQSComponent::URunEQSComponent(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer) {
    _queryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &URunEQSComponent::_onQueryFinished);
    PrimaryComponentTick.bCanEverTick = true;
}

void URunEQSComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    const auto aiController = Cast<AAIController>(GetOwner());
    APawn* queryOwner = nullptr;
    if (IsValid(aiController)) {
        queryOwner = aiController->GetPawn();
    }

    if (IsValid(queryOwner) && _eqsRequest.IsValid()) {
        const auto blackboardC = aiController->GetBlackboardComponent();
        _eqsRequest.Execute(*queryOwner, blackboardC, _queryFinishedDelegate);
    }
}

void URunEQSComponent::BeginPlay() {
    Super::BeginPlay();
    _eqsRequest.bInitialized = true;
}

void URunEQSComponent::_onQueryFinished(const TSharedPtr<FEnvQueryResult> result) {
    if (result->IsAborted()) {
        return;
    }

    const auto successfulQuery = result->IsSuccessful() && result->Items.Num() >= 0;
    if (successfulQuery) {
        _items = result->Items;
    } else if (_updateOnFail) {
        _items.Empty();
    }
}
