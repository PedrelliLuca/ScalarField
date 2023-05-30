// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FSMState.h"
#include "NewStateComponent.h"

#include "InventoryOpenState.generated.h"

/**
 * \brief In this state, the inventory is open.
 */
UCLASS()
class FSM_API UInventoryOpenState : public UObject, public IFSMState {
    GENERATED_BODY()

public:
    void SetPawn(TObjectPtr<APawn> subjectPawn) override;

    void OnEnter() override;
    void OnLeave() override;

    TScriptInterface<IFSMState> Tick(float deltaTime) override;

    TScriptInterface<IFSMState> TrySetMovementDestination(const FVector& movementDestination) override;
    TScriptInterface<IFSMState> TryStopMovement() override;

    FStateResponse_TryCastSkill TryCastSkillAtIndex(int32 index) override;
    TScriptInterface<IFSMState> TryAbort() override;
    TScriptInterface<IFSMState> TrySetSkillTarget(const FSkillTargetPacket& targetPacket) override;

    TScriptInterface<IFSMState> TryInteracting() override;
    TScriptInterface<IFSMState> TryToggleInventory() override;

private:
    TWeakObjectPtr<APawn> _subjectPawn = nullptr;
    TWeakObjectPtr<UNewStateComponent> _subjectStateC = nullptr;

    FDelegateHandle _inventoryWidgetCloseHandle{};
};
