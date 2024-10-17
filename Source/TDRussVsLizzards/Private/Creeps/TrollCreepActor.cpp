// TD Russ Vs Lizzards Game

#include "Creeps/TrollCreepActor.h"
#include "Components/WeaponComponent.h"
#include "Weapon/CudgelWeapon.h"

ATrollCreepActor::ATrollCreepActor()
{
    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
    check(IsValid(WeaponComponent));
    WeaponComponent->InitWeaponType(ACudgelWeapon::StaticClass());

    InitSkeletalMesh();

    InitAnimations();
}


void ATrollCreepActor::InitSkeletalMesh()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CreepMesh(
        TEXT("/Script/Engine.SkeletalMesh'/Game/Fantasy_Pack/Characters/Troll/Mesh/SK_Troll.SK_Troll'"));

    checkf(CreepMesh.Succeeded(), TEXT("Find Creep Mesh is not Succeeded "));

    SkeletalMeshComponent->SetSkeletalMesh(CreepMesh.Object.Get());
}

void ATrollCreepActor::InitAnimations()
{
    // IDLE
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepIdleAnimationRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Troll/Animations/Anim_Troll_Idle.Anim_Troll_Idle'"));

    checkf(CreepIdleAnimationRef.Succeeded(), TEXT("Find Creep Idle Animation is not Succeeded "));

    CreepIdleAnimation = CreepIdleAnimationRef.Object;

    // WALK
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepWalkAnimationRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Troll/Animations/Anim_Troll_Walk.Anim_Troll_Walk'"));

    checkf(CreepWalkAnimationRef.Succeeded(), TEXT("Find Creep Walk Animation is not Succeeded "));

    CreepWalkAnimation = CreepWalkAnimationRef.Object;

    // RUN
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepRunAnimRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Troll/Animations/Anim_Troll_Run.Anim_Troll_Run'"));

    checkf(CreepRunAnimRef.Succeeded(), TEXT("Find Creep Walk Animation is not Succeeded "));

    CreepRunAnimation = CreepRunAnimRef.Object;
}
