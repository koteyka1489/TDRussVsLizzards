// TD Russ Vs Lizzards Game

#include "Creeps/TrollCreepActor.h"
#include "Components/WeaponComponent.h"
#include "Weapon/CudgelWeapon.h"
#include "SkeletalMeshComponentBudgeted.h"

ATrollCreepActor::ATrollCreepActor()
{
    PrimaryActorTick.bCanEverTick = false;
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
    const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("/Script/Engine.AnimBlueprint'/Game/Animations/ABP_Troll.ABP_Troll'"));
    checkf(AnimBP.Succeeded(), TEXT("Find Creep ABP is not Succeeded "));
    SkeletalMeshComponent->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);
    
}
