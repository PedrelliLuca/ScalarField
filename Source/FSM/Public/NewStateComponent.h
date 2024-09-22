// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "FSMState.h"
#include "SkillTarget.h"

#include "NewStateComponent.generated.h"

UCLASS(ClassGroup = (StateMachine), meta = (BlueprintSpawnableComponent))
class FSM_API UNewStateComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UNewStateComponent();

    void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

    void TrySetMovementDestination(FVector const& movementDestination);
    void TryStopMovement();

    TOptional<FSkillCastResult> TryCastSkillAtIndex(uint32 index);
    void TryAbort();
    TOptional<TVariant<FSkillTargetingResult, FSkillCastResult>> TrySetSkillTarget(FSkillTargetPacket const& targetPacket);

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
