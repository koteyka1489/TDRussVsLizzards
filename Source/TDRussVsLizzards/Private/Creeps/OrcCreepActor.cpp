// TD Russ Vs Lizzards Game

#include "Creeps/OrcCreepActor.h"
#include "Components/WeaponComponent.h"
#include "Weapon/HummerWeapon.h"

AOrcCreepActor::AOrcCreepActor()
{
    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
    check(IsValid(WeaponComponent));
    WeaponComponent->InitWeaponType(AHummerWeapon::StaticClass());

    InitSkeletalMesh();

    InitAnimations();
}


void AOrcCreepActor::InitSkeletalMesh()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CreepMesh(
        TEXT("/Script/Engine.SkeletalMesh'/Game/Fantasy_Pack/Characters/Orc_Hummer/Mesh/SK_Orc_Hummer.SK_Orc_Hummer'"));

    checkf(CreepMesh.Succeeded(), TEXT("Find Creep Mesh is not Succeeded "));

    SkeletalMeshComponent->SetSkeletalMesh(CreepMesh.Object.Get());
}

void AOrcCreepActor::InitAnimations()
{

    // IDLE
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepIdleAnimationRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/Anim_Orc_Hummer_Idle.Anim_Orc_Hummer_Idle'"));

    checkf(CreepIdleAnimationRef.Succeeded(), TEXT("Find Creep Idle Animation is not Succeeded "));

    CreepIdleAnimation = CreepIdleAnimationRef.Object;

    // WALK
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepWalkAnimationRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/Anim_Orc_Hummer_Walk.Anim_Orc_Hummer_Walk'"));

    checkf(CreepWalkAnimationRef.Succeeded(), TEXT("Find Creep Walk Animation is not Succeeded "));

    CreepWalkAnimation = CreepWalkAnimationRef.Object;

    // RUN
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepRunAnimRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/Anim_Orc_Hummer_Run.Anim_Orc_Hummer_Run'"));

    checkf(CreepRunAnimRef.Succeeded(), TEXT("Find Creep Walk Animation is not Succeeded "));

    CreepRunAnimation = CreepRunAnimRef.Object;
}
