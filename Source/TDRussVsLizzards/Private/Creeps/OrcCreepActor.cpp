// TD Russ Vs Lizzards Game

#include "Creeps/OrcCreepActor.h"
#include "Components/WeaponComponent.h"
#include "Weapon/HummerWeapon.h"
#include "SkeletalMeshComponentBudgeted.h"

AOrcCreepActor::AOrcCreepActor()
{
    PrimaryActorTick.bCanEverTick = false;

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
    const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/ABP_Orc.ABP_Orc'"));
    checkf(AnimBP.Succeeded(), TEXT("Find Creep ABP is not Succeeded "));
    SkeletalMeshComponent->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);
}
