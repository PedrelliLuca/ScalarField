// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "FSMState.h"
#include "SkillTarget.h"

#include "NewStateComponent.generated.h"

UCLASS()
class FSM_API UNewStateComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UNewStateComponent();

    void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

    void TrySetMovementDestination(const FVector& movementDestination);
    void TryStopMovement();

    void TryCastSkillAtIndex(uint32 index);
    void TryAbort();
    void TrySetSkillTarget(const FSkillTargetPacket& targetPacket);

    void TryInteracting();
    void TryToggleInventory();

protected:
    void BeginPlay() override;

private:
    void _performStateTransition(TScriptInterface<IFSMState>&& newState);

    UPROPERTY()
    TScriptInterface<IFSMState> _state = nullptr;

    TWeakObjectPtr<APawn> _ownerPawn = nullptr;
};
