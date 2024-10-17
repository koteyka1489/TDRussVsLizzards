// TD Russ Vs Lizzards Game

#include "Creeps/BarbarianCreepActor.h"
#include "Components/WeaponComponent.h"
#include "Weapon/AxelWeapon.h"

ABarbarianCreepActor::ABarbarianCreepActor()
{
    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
    check(IsValid(WeaponComponent));
    WeaponComponent->InitWeaponType(AAxelWeapon::StaticClass());

    InitSkeletalMesh();

    InitAnimations();
}

void ABarbarianCreepActor::InitSkeletalMesh()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CreepMesh(
        TEXT("/Script/Engine.SkeletalMesh'/Game/Fantasy_Pack/Characters/Barbarian/Mesh/SK_Barbarian_Full.SK_Barbarian_Full'"));

    checkf(CreepMesh.Succeeded(), TEXT("Find Creep Mesh is not Succeeded "));

    SkeletalMeshComponent->SetSkeletalMesh(CreepMesh.Object.Get());
}

void ABarbarianCreepActor::InitAnimations()
{

    // IDLE
    static ConstructorHelpers::FObjectFinder<UAnimMontage> CreepIdleAnimationRef(
        TEXT("/Script/Engine.AnimMontage'/Game/Fantasy_Pack/Animations/1With_Weapon/AnimMontages/AM_Anim_Warrior_Idle.AM_Anim_Warrior_Idle'"));

    checkf(CreepIdleAnimationRef.Succeeded(), TEXT("Find Creep Idle Animation is not Succeeded "));

    CreepIdleAnimation = CreepIdleAnimationRef.Object;

    // WALK
    static ConstructorHelpers::FObjectFinder<UAnimMontage> CreepWalkAnimationRef(
        TEXT("/Script/Engine.AnimMontage'/Game/Fantasy_Pack/Animations/1With_Weapon/AnimMontages/AM_Anim_Warrior_Walk.AM_Anim_Warrior_Walk'"));

    checkf(CreepWalkAnimationRef.Succeeded(), TEXT("Find Creep Walk Animation is not Succeeded "));

    CreepWalkAnimation = CreepWalkAnimationRef.Object;

    // RUN
    static ConstructorHelpers::FObjectFinder<UAnimMontage> CreepRunAnimRef(
        TEXT("/Script/Engine.AnimMontage'/Game/Fantasy_Pack/Animations/1With_Weapon/AnimMontages/AM_Anim_Warrior_Run.AM_Anim_Warrior_Run'"));

    checkf(CreepRunAnimRef.Succeeded(), TEXT("Find Creep Walk Animation is not Succeeded "));

    CreepRunAnimation = CreepRunAnimRef.Object;
}
