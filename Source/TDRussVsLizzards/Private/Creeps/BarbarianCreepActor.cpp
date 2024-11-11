// TD Russ Vs Lizzards Game

#include "Creeps/BarbarianCreepActor.h"
#include "Components/WeaponComponent.h"
#include "Weapon/AxelWeapon.h"
#include "SkeletalMeshComponentBudgeted.h"

ABarbarianCreepActor::ABarbarianCreepActor()
{
    PrimaryActorTick.bCanEverTick = false;

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
    const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/ABP_UE4Skeleton.ABP_UE4Skeleton'"));
    checkf(AnimBP.Succeeded(), TEXT("Find Creep ABP is not Succeeded "));
    SkeletalMeshComponent->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);
}
