
#include "FireGlobeSkill.h"

#include "GameFramework/SpringArmComponent.h"

void UFireGlobeSkill::ExecuteCast(TObjectPtr<AActor> caster) {
    const auto& fireGlobeSpawner = _getFollowerActorSpawners()[0];

    const auto target = _getActorTargets().Last().Target;

    const TObjectPtr<USpringArmComponent> spawnSpringArm = NewObject<USpringArmComponent>(target.Get(), TEXT("Globe SpringArm"));
    spawnSpringArm->SetupAttachment(target->GetRootComponent());

    // The point where we have to spawn the globe relative to the target, it's also the point where the 2nd end of the arm lies
    const FVector globeLocation = fireGlobeSpawner.Transform.GetLocation();

    // The spring sits on the vector that goes from the target's root to the globeLocation.
    // About the minus sign:
    // In the spring's reference frame, the spring elongates in the -x direction. Therefore, we need to make the spring point to
    // the location that is the opposite of the want we want the second end to be in.
    spawnSpringArm->SetRelativeRotation((-globeLocation).Rotation());
    spawnSpringArm->TargetArmLength = globeLocation.Length();

    spawnSpringArm->RegisterComponent();

    const TWeakObjectPtr<AActor> spawnActor = GetWorld()->SpawnActor<AActor>(fireGlobeSpawner.ActorClass, fireGlobeSpawner.Transform * target->GetTransform());
    spawnActor->AttachToComponent(spawnSpringArm, FAttachmentTransformRules::KeepWorldTransform, spawnSpringArm->SocketName);

    FTimerHandle timerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        timerHandle,
        [spawnSpringArm = TWeakObjectPtr<USpringArmComponent>(spawnSpringArm), spawnActor]() {
            if (spawnActor.IsValid()) {
                spawnActor->Destroy();
            }

            if (spawnSpringArm.IsValid()) {
                spawnSpringArm->DestroyComponent();
            }
        },
        fireGlobeSpawner.SpawnDuration, false);

    _startCooldown();
}

bool UFireGlobeSkill::IsValidTarget(int32 targetIndex, TObjectPtr<AActor> target) const {
    /* TODO: for the future, implement a faction component that stores the owner's faction and lists its allies and
     * enemies. Some skills can only be casted on allies, other only on enemies.
     */
    return Super::IsValidTarget(targetIndex, target) && _isControlled(MoveTemp(target));
}

#if DO_CHECK
void UFireGlobeSkill::CheckParametersSanity() const {
    check(_getFollowerActorSpawners().Num() == 1);
}
#endif

bool UFireGlobeSkill::_isControlled(TObjectPtr<AActor> target) const {
    if (const auto pawn = Cast<APawn>(target)) {
        if (const auto controller = pawn->GetController()) {
            return true;
        }
    }
    return false;
}
