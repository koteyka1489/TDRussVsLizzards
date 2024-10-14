// TD Russ Vs Lizzards Game

#include "Weapon/AxelWeapon.h"

AAxelWeapon::AAxelWeapon()
{
    SetWeaponMesh();
}

void AAxelWeapon::SetWeaponMesh()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(
        TEXT("/Script/Engine.StaticMesh'/Game/Fantasy_Pack/Characters/Barbarian/Mesh/SM_Axel.SM_Axel'"));

    checkf(WeaponMesh.Succeeded(), TEXT("Find Weapon Mesh not Succeeded"));

    WeaponStaticMesh->SetStaticMesh(WeaponMesh.Object);
}
