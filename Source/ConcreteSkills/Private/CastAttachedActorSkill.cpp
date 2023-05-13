// Fill out your copyright notice in the Description page of Project Settings.

#include "CastAttachedActorSkill.h"

#include "SkillSpawnedEntityInterface.h"

void UCastAttachedActorSkill::_skillCast() {
    const TObjectPtr<USpringArmComponent> spawnSpringArm = NewObject<USpringArmComponent>(_getCaster().Get(), TEXT("SkillSpringArm"));
    spawnSpringArm->bDoCollisionTest = false;
    spawnSpringArm->SetVisibility(true);
    spawnSpringArm->SetupAttachment(_getCaster()->GetRootComponent());

    // The point where we have to spawn the globe relative to the target, it's also the point where the 2nd end of the arm lies
    const auto location = _actorToTarget.GetLocation();

    // We want the spring arm to represent the vector that goes from the target's root to the location.
    /* A spring arm elongates along the direction opposite to its reference frame x axis. So, to make the spring arm elongate in the
     * (+location).Rotation() direction, we must rotate it so that its x axis is equivalent to (-location).Rotation(). */
    const auto relativeRotation = (-location).Rotation();
    spawnSpringArm->SetRelativeRotation(relativeRotation);
    spawnSpringArm->TargetArmLength = location.Length();

    spawnSpringArm->RegisterComponent();
    _spawnSpringArm = spawnSpringArm;

    // Actor deferred spawn to set the caster >>>>>>>>>>
    const auto spawnedEntityWorldTransform = _actorToTarget * _getCaster()->GetTransform();
    const auto spawnActor = GetWorld()->SpawnActorDeferred<AActor>(_actorClass, spawnedEntityWorldTransform);

    // A simple cast like the following will return null if the interface is implemented in BP instead of C++. This is because the C++ side has no idea the
    // actor implements the interface in such cases, as explained here https://forums.unrealengine.com/t/c-interface-implemented-in-bp-is-null/491758.
    // Why may I want the implementation on the BP side?
    // Some skill-spawned actors, like the Fire Globe, don't require any custom logic: you just want them to implement the interface so that it's selectable
    // from the dropdown menu of FActorSpawnerParameters::ActorClass. For cases like these, the BP-side implementation is perfect, and I don't care about
    // calling the interface-specific functions since there is no important logic these skill-spawned entities have to execute.
    const TWeakInterfacePtr<ISkillSpawnedEntity> skillSpawnedEntity = Cast<ISkillSpawnedEntity>(spawnActor);
    if (skillSpawnedEntity.IsValid()) {
        skillSpawnedEntity->SetCaster(_getCaster());
    }

    spawnActor->FinishSpawning(spawnedEntityWorldTransform);
    _spawnActor = spawnActor;
    // <<<<<<<<<<

    spawnActor->AttachToComponent(spawnSpringArm, FAttachmentTransformRules::KeepWorldTransform, spawnSpringArm->SocketName);
}

void UCastAttachedActorSkill::_skillChannelingTick(const float deltaTime) {
    // This skill doesn't do anything on channeling
}

void UCastAttachedActorSkill::_skillAbort() {
    // The spawn actor and spring might have been destroyed by something else, or because the target died. There is no reason to assume it's still alive, which
    // is why we don't check here.
    if (_spawnActor.IsValid()) {
        _spawnActor->Destroy();
    }

    if (_spawnSpringArm.IsValid()) {
        _spawnSpringArm->DestroyComponent();
    }
}
