// TD Russ Vs Lizzards Game

#include "Creeps/OrcCreepActor.h"

AOrcCreepActor::AOrcCreepActor()
{
    InitSkeletalMesh();

    InitAnimations();
}

void AOrcCreepActor::InitSkeletalMesh()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CreepMesh(
        TEXT("/Script/Engine.SkeletalMesh'/Game/Fantasy_Pack/Characters/Orc_Hummer/Mesh/SK_Orc_Hummer.SK_Orc_Hummer'"));
    if (CreepMesh.Succeeded())
    {
        SkeletalMeshComponent->SetSkeletalMesh(CreepMesh.Object.Get());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Wrong Reference of Skeletal Mesh"));
        checkNoEntry();
    }
}

void AOrcCreepActor::InitAnimations()
{
    // IDLE
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepIdleAnimationRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/Anim_Orc_Hummer_Idle.Anim_Orc_Hummer_Idle'"));
    if (CreepIdleAnimationRef.Succeeded())
    {
        CreepIdleAnimation = CreepIdleAnimationRef.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Wrong Reference of Animation"));
        checkNoEntry();
    }

    // WALK
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepWalkAnimationRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/Anim_Orc_Hummer_Walk.Anim_Orc_Hummer_Walk'"));
    if (CreepWalkAnimationRef.Succeeded())
    {
        CreepWalkAnimation = CreepWalkAnimationRef.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Wrong Reference of Animation"));
        checkNoEntry();
    }

    // RUN
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepRunAnimRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/Anim_Orc_Hummer_Run.Anim_Orc_Hummer_Run'"));
    if (CreepRunAnimRef.Succeeded())
    {
        CreepRunAnimation = CreepRunAnimRef.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Wrong Reference of Animation"));
        checkNoEntry();
    }
}
