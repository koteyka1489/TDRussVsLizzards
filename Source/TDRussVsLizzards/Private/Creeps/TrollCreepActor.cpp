// TD Russ Vs Lizzards Game

#include "Creeps/TrollCreepActor.h"

ATrollCreepActor::ATrollCreepActor() 
{
    InitSkeletalMesh();

    InitAnimations();
}

void ATrollCreepActor::InitSkeletalMesh()
{
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CreepMesh(
        TEXT("/Script/Engine.SkeletalMesh'/Game/Fantasy_Pack/Characters/Troll/Mesh/SK_Troll.SK_Troll'"));
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

void ATrollCreepActor::InitAnimations()
{  
    // IDLE
    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepIdleAnimationRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Troll/Animations/Anim_Troll_Idle.Anim_Troll_Idle'"));
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
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Troll/Animations/Anim_Troll_Walk.Anim_Troll_Walk'"));
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
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Troll/Animations/Anim_Troll_Run.Anim_Troll_Run'"));
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
