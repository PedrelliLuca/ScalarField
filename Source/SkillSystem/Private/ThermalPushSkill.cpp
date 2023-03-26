// Fill out your copyright notice in the Description page of Project Settings.

#include "ThermalPushSkill.h"

#include "SkillSpawnedEntityInterface.h"
#include "UObject/WeakInterfacePtr.h"

void UThermalPushSkill::ExecuteCast(TObjectPtr<AActor> caster) {
    const auto& thermalWindSpawner = _getFollowerActorSpawners().Last();

    const TObjectPtr<USpringArmComponent> spawnSpringArm = NewObject<USpringArmComponent>(caster.Get(), TEXT("Skill SpringArm"));
    spawnSpringArm->bDoCollisionTest = false;
    spawnSpringArm->SetupAttachment(caster->GetRootComponent());

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
    const TWeakInterfacePtr<ISkillSpawnedEntity> skillSpawnedEntity = Cast<ISkillSpawnedEntity>(spawnActor);
    // This should be granted by the UPROPERTY on FActorSpawnerParameters::ActorClass
    check(skillSpawnedEntity.IsValid());
    skillSpawnedEntity->SetCaster(caster);
    spawnActor->FinishSpawning(socketTransform);
    // <<<<<<<<<<

    spawnActor->AttachToComponent(spawnSpringArm, FAttachmentTransformRules::KeepWorldTransform, spawnSpringArm->SocketName);
    _spawnActor = spawnActor;

    _startCooldown();
}

void UThermalPushSkill::Abort() {
    Super::Abort();
    _cleanupCallback();
}

void UThermalPushSkill::_cleanupCallback() {
    GetWorld()->GetTimerManager().ClearTimer(_timerHandle);

    if (_spawnActor.IsValid()) {
        _spawnActor->Destroy();
    }

    if (_spawnSpringArm.IsValid()) {
        _spawnSpringArm->DestroyComponent();
    }
}
