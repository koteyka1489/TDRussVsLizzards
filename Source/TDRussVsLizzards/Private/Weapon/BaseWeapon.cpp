// TD Russ Vs Lizzards Game

#include "Weapon/BaseWeapon.h"
#include "Engine/StaticMesh.h"

ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("UStaticMeshComponent");
    SetRootComponent(WeaponStaticMesh);

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

void ABaseWeapon::SetWeaponMesh() {}
