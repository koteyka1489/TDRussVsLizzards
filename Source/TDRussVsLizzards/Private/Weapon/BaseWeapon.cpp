// TD Russ Vs Lizzards Game

#include "Weapon/BaseWeapon.h"
#include "Engine/StaticMesh.h"

ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("UStaticMeshComponent");
    SetRootComponent(WeaponStaticMesh);

    SetWeaponMesh();

    WeaponStaticMesh->SetVisibility(true, true);
    WeaponStaticMesh->SetSimulatePhysics(false);
    WeaponStaticMesh->CastShadow                   = 1u;
    WeaponStaticMesh->bVisibleInRayTracing         = true;
    WeaponStaticMesh->bVisibleInReflectionCaptures = true;
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseWeapon::SetWeaponMesh()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(
        TEXT("/Script/Engine.StaticMesh'/Game/Fantasy_Pack/Characters/Orc_Hummer/Mesh/SM_Hummer.SM_Hummer'"));
    if (WeaponMesh.Succeeded())
    {
        WeaponStaticMesh->SetStaticMesh(WeaponMesh.Object);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Wrong Reference of Skeletal Mesh"));
        checkNoEntry();
    }
}
