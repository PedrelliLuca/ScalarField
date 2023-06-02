// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "RunEQSComponent.generated.h"

UCLASS()
class AICONTROLLERS_API URunEQSComponent : public UActorComponent {
    GENERATED_BODY()

public:
    URunEQSComponent(const FObjectInitializer& objectInitializer);
    
    void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

protected:
    void BeginPlay() override;
    
private:
    void _onQueryFinished(TSharedPtr<FEnvQueryResult> result);
    
    UPROPERTY(Category = "EQS", EditAnywhere)
    FEQSParametrizedQueryExecutionRequest _eqsRequest;

    UPROPERTY(Category = "EQS", EditAnywhere)
    bool _updateOnFail = false;

    FQueryFinishedSignature _queryFinishedDelegate;

    TArray<FEnvQueryItem> _items;
};
