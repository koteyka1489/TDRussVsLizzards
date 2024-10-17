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
    static ConstructorHelpers::FObjectFinder<UAnimMontage> CreepIdleAnimationRef(
        TEXT("/Script/Engine.AnimMontage'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/AnimMontages/AM_Anim_Orc_Hummer_Idle.AM_Anim_Orc_Hummer_Idle'"));

    checkf(CreepIdleAnimationRef.Succeeded(), TEXT("Find Creep Idle Animation is not Succeeded "));

    CreepIdleAnimation = CreepIdleAnimationRef.Object;

    // WALK
    static ConstructorHelpers::FObjectFinder<UAnimMontage> CreepWalkAnimationRef(
        TEXT("/Script/Engine.AnimMontage'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/AnimMontages/AM_Anim_Orc_Hummer_Walk.AM_Anim_Orc_Hummer_Walk'"));

    checkf(CreepWalkAnimationRef.Succeeded(), TEXT("Find Creep Walk Animation is not Succeeded "));

    CreepWalkAnimation = CreepWalkAnimationRef.Object;

    // RUN
    static ConstructorHelpers::FObjectFinder<UAnimMontage> CreepRunAnimRef(
        TEXT("/Script/Engine.AnimMontage'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/AnimMontages/AM_Anim_Orc_Hummer_Run.AM_Anim_Orc_Hummer_Run'"));

    checkf(CreepRunAnimRef.Succeeded(), TEXT("Find Creep Walk Animation is not Succeeded "));

    CreepRunAnimation = CreepRunAnimRef.Object;
}
