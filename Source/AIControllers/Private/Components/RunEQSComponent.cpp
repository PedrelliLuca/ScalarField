// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/RunEQSComponent.h"

#include "AIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

URunEQSComponent::URunEQSComponent(FObjectInitializer const& objectInitializer)
    : Super(objectInitializer) {
    _queryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &URunEQSComponent::_onQueryFinished);
    PrimaryComponentTick.bCanEverTick = true;
}

void URunEQSComponent::TickComponent(float const deltaTime, ELevelTick const tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    _secondsSinceLastQuery += deltaTime;
    if (_secondsSinceLastQuery < _secondsBetweenQueries) {
        return;
    }

    auto const aiController = Cast<AAIController>(GetOwner());
    APawn* queryOwner = nullptr;
    if (IsValid(aiController)) {
        queryOwner = aiController->GetPawn();
    }

    if (IsValid(queryOwner) && _eqsRequest.IsValid()) {
        auto const blackboardC = aiController->GetBlackboardComponent();
        _eqsRequest.Execute(*queryOwner, blackboardC, _queryFinishedDelegate);
    }

    _secondsSinceLastQuery = 0.0f;
}

bool URunEQSComponent::DidQueryReturnItems(int32 const requiredItems) const {
    return _queryResult.IsValid() ? _queryResult->Items.Num() >= requiredItems : requiredItems == 0;
}

TOptional<TSubclassOf<UEnvQueryItemType>> URunEQSComponent::GetQueryItemsType() const {
    auto queryItemsType = TOptional<TSubclassOf<UEnvQueryItemType>>{};

    if (!_queryResult.IsValid()) {
        return queryItemsType;
    }

    return _queryResult->ItemType;
}

void URunEQSComponent::BeginPlay() {
    Super::BeginPlay();
    _eqsRequest.bInitialized = true;
}

void URunEQSComponent::_onQueryFinished(TSharedPtr<FEnvQueryResult> result) {
    if (result->IsAborted()) {
        return;
    }

    // This is because of _didQueryItemsChange, see below. TODO: refactor that function to also work with vector type
    check(result->ItemType == UEnvQueryItemType_Actor::StaticClass());

    auto const successfulQuery = result->IsSuccessful() && result->Items.Num() >= 0;

    if (_didQueryItemsChange(result)) {
        _onQueryResultChange.Broadcast();
    }

    if (successfulQuery || _updateOnFail) {
        _queryResult = MoveTemp(result);
    }
}

bool URunEQSComponent::_didQueryItemsChange(TSharedPtr<FEnvQueryResult> const& result) const {
    if (!_queryResult.IsValid() || result->Items.Num() != _queryResult->Items.Num()) {
        return true;
    }

    // Heavily inspired by template<class TDataClass> bool UBlackboardComponent::SetValue() (BlackboardComponent.h) calling bool
    // UBlackboardKeyType_Object::SetValue() (BlackboardKeyType_Object.cpp) calling template<typename T> static bool SetWeakObjectInMemory()
    // (BlackboardKeyType.h)
    for (int32 i = 0; i < _queryResult->Items.Num(); ++i) {
        auto const newRawData = result->RawData.GetData() + result->Items[i].DataOffset;
        auto const oldRawData = _queryResult->RawData.GetData() + _queryResult->Items[i].DataOffset;

        // TODO: refactor this to also work with vectors
        auto const newValue = *reinterpret_cast<TWeakObjectPtr<UObject>*>(newRawData);
        auto const oldValue = *reinterpret_cast<TWeakObjectPtr<UObject>*>(oldRawData);

        if (newValue != oldValue) {
            return true;
        }
    }

    return false;
}
