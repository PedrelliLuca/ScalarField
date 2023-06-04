// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "RunEQSComponent.generated.h"

class FQueryItemsIterator {
public:
    FQueryItemsIterator(TSharedPtr<FEnvQueryResult>&& result)
        : _result(MoveTemp(result))
        , _currentIdx(0) {}

    bool IsDone() const { return _result->Items.Num() == _currentIdx; }

    const uint8* Current() const { return _result->RawData.GetData() + _result->Items[_currentIdx].DataOffset; }
    const uint8* operator*() const { return Current(); }

    void Next() { ++_currentIdx; }
    void operator++() { Next(); } // Pre-increment operator

private:
    TSharedPtr<FEnvQueryResult> _result;
    int32 _currentIdx;
};

DECLARE_MULTICAST_DELEGATE(FOnQueryResultChange);

UCLASS()
class AICONTROLLERS_API URunEQSComponent : public UActorComponent {
    GENERATED_BODY()

public:
    URunEQSComponent(const FObjectInitializer& objectInitializer);

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

    FQueryItemsIterator GetQueryItemsIterator();

    bool DidQueryReturnItems(const int32 requiredItems) const;

    FOnQueryResultChange& OnQueryResultChange() { return _onQueryResultChange; }

protected:
    void BeginPlay() override;

private:
    void _onQueryFinished(TSharedPtr<FEnvQueryResult> result);

    /* Note: This only works with EQs returning actors.
     * TODO: refactor this to also work with Environment Queries that return locations */
    bool _didQueryItemsChange(const TSharedPtr<FEnvQueryResult>& result) const;

    UPROPERTY(Category = "EQS", EditAnywhere)
    FEQSParametrizedQueryExecutionRequest _eqsRequest;

    UPROPERTY(Category = "EQS", EditAnywhere)
    bool _updateOnFail = false;

    UPROPERTY(Category = "EQS", EditAnywhere)
    float _secondsBetweenQueries = 1.0f;

    FQueryFinishedSignature _queryFinishedDelegate;

    TSharedPtr<FEnvQueryResult> _queryResult;

    float _secondsSinceLastQuery = 0.0f;

    FOnQueryResultChange _onQueryResultChange;
};
