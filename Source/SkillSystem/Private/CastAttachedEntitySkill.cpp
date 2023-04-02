// Fill out your copyright notice in the Description page of Project Settings.

#include "CastAttachedEntitySkill.h"

#include "SkillSpawnedEntityInterface.h"
#include "UObject/WeakInterfacePtr.h"

void UCastAttachedEntitySkill::ExecuteCast(TObjectPtr<AActor> caster) {
    const auto& thermalWindSpawner = _getFollowerActorSpawners().Last();

    auto target = caster;
    if (RequiresTarget()) {
        target = _getActorTargets().Last().Target.Get();
    }

    const TObjectPtr<USpringArmComponent> spawnSpringArm = NewObject<USpringArmComponent>(target.Get(), TEXT("Skill SpringArm"));
    spawnSpringArm->bDoCollisionTest = false;
    spawnSpringArm->SetupAttachment(target->GetRootComponent());

    // The point where we have to spawn the globe relative to the target, it's also the point where the 2nd end of the arm lies
    const auto windLocation = thermalWindSpawner.Transform.GetLocation();

    // The spring sits on the vector that goes from the target's root to the globeLocation.
    // About the minus sign:
    // In the spring's reference frame, the spring elongates in the -x direction. Therefore, we need to make the spring point to
    // the location that is the opposite of the want we want the second end to be in.
    spawnSpringArm->SetRelativeRotation((-windLocation).Rotation());
    spawnSpringArm->TargetArmLength = windLocation.Length();

    spawnSpringArm->RegisterComponent();
    _spawnSpringArm = spawnSpringArm;

    // Actor deferred spawn to set the caster >>>>>>>>>>
    const auto socketTransform = spawnSpringArm->GetSocketTransform(spawnSpringArm->SocketName, ERelativeTransformSpace::RTS_World);
    const auto spawnActor = GetWorld()->SpawnActorDeferred<AActor>(thermalWindSpawner.ActorClass, socketTransform);

    // A simple cast like the following will return null if the interface is implemented in BP instead of C++. This is because the C++ side has no idea the
    // actor implements the interface in such cases, as explained here https://forums.unrealengine.com/t/c-interface-implemented-in-bp-is-null/491758.
    // Why may I want the implementation on the BP side?
    // Some skill-spawned actors, like the Fire Globe, don't require any custom logic: you just want them to implement the interface so that it's selectable
    // from the dropdown menu of FActorSpawnerParameters::ActorClass. For cases like these, the BP-side implementation is perfect, and I don't care about
    // calling the interface-specific functions, bceause there is no important logic these skill-spawned entities have to execute.
    const TWeakInterfacePtr<ISkillSpawnedEntity> skillSpawnedEntity = Cast<ISkillSpawnedEntity>(spawnActor);
    if (skillSpawnedEntity.IsValid()) {
        skillSpawnedEntity->SetCaster(caster);
    }

    spawnActor->FinishSpawning(socketTransform);
    // <<<<<<<<<<

    spawnActor->AttachToComponent(spawnSpringArm, FAttachmentTransformRules::KeepWorldTransform, spawnSpringArm->SocketName);
    _spawnActor = spawnActor;

    _startCooldown();
}

void UCastAttachedEntitySkill::Abort() {
    Super::Abort();
    _cleanupCallback();
}

void UCastAttachedEntitySkill::_cleanupCallback() {
    GetWorld()->GetTimerManager().ClearTimer(_timerHandle);

    if (_spawnActor.IsValid()) {
        _spawnActor->Destroy();
    }

    if (_spawnSpringArm.IsValid()) {
        _spawnSpringArm->DestroyComponent();
    }
}
