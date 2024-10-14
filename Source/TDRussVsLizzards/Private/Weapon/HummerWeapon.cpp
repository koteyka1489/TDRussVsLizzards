// TD Russ Vs Lizzards Game

#include "Weapon/HummerWeapon.h"

AHummerWeapon::AHummerWeapon()
{
    SetWeaponMesh();
}

void AHummerWeapon::SetWeaponMesh()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(
        TEXT("/Script/Engine.StaticMesh'/Game/Fantasy_Pack/Characters/Orc_Hummer/Mesh/SM_Hummer.SM_Hummer'"));

    checkf(WeaponMesh.Succeeded(), TEXT("Find Weapon Mesh not Succeeded"));

    WeaponStaticMesh->SetStaticMesh(WeaponMesh.Object);
}
