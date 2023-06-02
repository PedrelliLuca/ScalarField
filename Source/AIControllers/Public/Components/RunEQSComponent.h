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

    // Pre-increment operator
    void operator++() { Next(); }

    void Next() { ++_currentIdx; }

private:
    TSharedPtr<FEnvQueryResult> _result;
    int32 _currentIdx;
};

UCLASS()
class AICONTROLLERS_API URunEQSComponent : public UActorComponent {
    GENERATED_BODY()

public:
    URunEQSComponent(const FObjectInitializer& objectInitializer);

    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

    FQueryItemsIterator GetQueryItemsIterator();

protected:
    void BeginPlay() override;

private:
    void _onQueryFinished(TSharedPtr<FEnvQueryResult> result);

    UPROPERTY(Category = "EQS", EditAnywhere)
    FEQSParametrizedQueryExecutionRequest _eqsRequest;

    UPROPERTY(Category = "EQS", EditAnywhere)
    bool _updateOnFail = false;

    FQueryFinishedSignature _queryFinishedDelegate;

    TSharedPtr<FEnvQueryResult> _queryResult;
};
