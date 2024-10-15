// TD Russ Vs Lizzards Game

#include "Weapon/BaseWeapon.h"
#include "Engine/StaticMesh.h"

ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("UStaticMeshComponent");
    SetRootComponent(WeaponStaticMesh);


    check(IsValid(WeaponStaticMesh));
    WeaponStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    WeaponStaticMesh->SetVisibility(true, true);
    WeaponStaticMesh->SetSimulatePhysics(false);
    WeaponStaticMesh->CastShadow                   = 0u;
    WeaponStaticMesh->bVisibleInRayTracing         = false;
    WeaponStaticMesh->bVisibleInReflectionCaptures = false;
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseWeapon::SetWeaponMesh() {}
