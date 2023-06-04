// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/RunEQSComponent.h"

#include "AIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

URunEQSComponent::URunEQSComponent(const FObjectInitializer& objectInitializer)
    : Super(objectInitializer) {
    _queryFinishedDelegate = FQueryFinishedSignature::CreateUObject(this, &URunEQSComponent::_onQueryFinished);
    PrimaryComponentTick.bCanEverTick = true;
}

void URunEQSComponent::TickComponent(const float deltaTime, const ELevelTick tickType, FActorComponentTickFunction* const thisTickFunction) {
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    _secondsSinceLastQuery += deltaTime;
    if (_secondsSinceLastQuery < _secondsBetweenQueries) {
        return;
    }

    const auto aiController = Cast<AAIController>(GetOwner());
    APawn* queryOwner = nullptr;
    if (IsValid(aiController)) {
        queryOwner = aiController->GetPawn();
    }

    if (IsValid(queryOwner) && _eqsRequest.IsValid()) {
        const auto blackboardC = aiController->GetBlackboardComponent();
        _eqsRequest.Execute(*queryOwner, blackboardC, _queryFinishedDelegate);
    }

    _secondsSinceLastQuery = 0.0f;
}

FQueryItemsIterator URunEQSComponent::GetQueryItemsIterator() {
    return FQueryItemsIterator{MoveTemp(_queryResult)};
}

bool URunEQSComponent::DidQueryReturnItems(const int32 requiredItems) const {
    return _queryResult.IsValid() ? _queryResult->Items.Num() >= requiredItems : requiredItems == 0;
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

    const auto successfulQuery = result->IsSuccessful() && result->Items.Num() >= 0;

    // result->Items != _queryResult->Items
    if (_didQueryItemsChange(result)) {
        _onQueryResultChange.Broadcast();
    }

    if (successfulQuery || _updateOnFail) {
        _queryResult = MoveTemp(result);
    }
}

bool URunEQSComponent::_didQueryItemsChange(const TSharedPtr<FEnvQueryResult>& result) const {
    if (!_queryResult.IsValid() || result->Items.Num() != _queryResult->Items.Num()) {
        return true;
    }

    // Heavily inspired by template<class TDataClass> bool UBlackboardComponent::SetValue() (BlackboardComponent.h) calling bool
    // UBlackboardKeyType_Object::SetValue() (BlackboardKeyType_Object.cpp) calling template<typename T> static bool SetWeakObjectInMemory()
    // (BlackboardKeyType.h)
    for (int32 i = 0; i < _queryResult->Items.Num(); ++i) {
        const auto newRawData = result->RawData.GetData() + result->Items[i].DataOffset;
        const auto oldRawData = _queryResult->RawData.GetData() + _queryResult->Items[i].DataOffset;

        const auto newValue = *reinterpret_cast<TWeakObjectPtr<UObject>*>(newRawData);
        const auto oldValue = *reinterpret_cast<TWeakObjectPtr<UObject>*>(oldRawData);

        if (newValue != oldValue) {
            return true;
        }
    }

    return false;
}
