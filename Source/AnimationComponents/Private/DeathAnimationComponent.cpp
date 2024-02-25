// Fill out your copyright notice in the Description page of Project Settings.

#include "DeathAnimationComponent.h"

#include "AIController.h"
#include "Animation/AnimMontage.h"
#include "BrainComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "HealthComponent.h"

UDeathAnimationComponent::UDeathAnimationComponent() {
    PrimaryComponentTick.bCanEverTick = false;
}

void UDeathAnimationComponent::BeginPlay() {
    Super::BeginPlay();

    if (IsValid(_deathMontage)) {
        if (_ownerCharacter = Cast<ACharacter>(GetOwner()); _ownerCharacter.IsValid()) {
            if (UHealthComponent* const healthC = GetOwner()->FindComponentByClass<UHealthComponent>(); IsValid(healthC)) {
                healthC->OnDeath().AddUObject(this, &UDeathAnimationComponent::_playDeathMontage);
            } else {
                UE_LOG(LogTemp, Error, TEXT("%s: Owner does not have an UHealthComponent, death animation won't play"), *FString(__FUNCTION__));
            }
        } else {
            UE_LOG(LogTemp, Error, TEXT("%s: Owner is not an AACharacter, death animation won't play."), *FString(__FUNCTION__));
        }
    } else {
        UE_LOG(LogTemp, Error, TEXT("%s: Invalid DeathMontage, death animation won't play.\nDid you remember to set it?"), *FString(__FUNCTION__));
    }
}

// The parameter of this function is useless. However, due to an architectural error (see description of FOnDeath macro, UHealthComponent.h), it is necessary. I
// should remove this one day.
void UDeathAnimationComponent::_playDeathMontage(TObjectPtr<AActor> _) {
    // This component should be the one responsible for destroying the owner, how can it be already invalid?
    check(_ownerCharacter.IsValid());

    _ownerCharacter->GetMovementComponent()->StopMovementImmediately();

    // An alternative approach to avoid depending from AAIController and UBrainComponent would be to create a custom
    // UBrainComponent that binds to the OnDeath delegate of UHealthComponent to call StopLogic(). This custom brain could be
    // assigned during the AIController's construction. For the time being, this is not a big deal.
    if (AAIController* aiController = Cast<AAIController>(_ownerCharacter->GetController()); IsValid(aiController)) {
        aiController->GetBrainComponent()->StopLogic(FString{TEXT("Death")});
    }

    constexpr float playRate = 1.0f;
    const bool playedSuccessfully = _ownerCharacter->PlayAnimMontage(_deathMontage, playRate) > 0.0f;
    if (playedSuccessfully) {
        UAnimInstance* const animInstance = _ownerCharacter->GetMesh()->GetAnimInstance();

        check(!_montageBlendingOutStartDelegate.IsBound());
        _montageBlendingOutStartDelegate.BindUObject(this, &UDeathAnimationComponent::_onDeathMontageEnded);

        // Montage_SetEndDelegate() is not good, because the end of the delegate is way after its blend out time. In other words, you'd see the character going
        // back to the idle animation.
        animInstance->Montage_SetBlendingOutDelegate(_montageBlendingOutStartDelegate, _deathMontage);
    }
}

void UDeathAnimationComponent::_onDeathMontageEnded(UAnimMontage* const montage, const bool bInterrupted) const {
    // This component should be the one responsible for destroying the owner, how can it be already invalid?
    check(_ownerCharacter.IsValid());
    _ownerCharacter->Destroy();
}
