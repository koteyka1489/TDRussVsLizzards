// TD Russ Vs Lizzards Game


#include "Weapon/CudgelWeapon.h"

ACudgelWeapon::ACudgelWeapon()
{
    SetWeaponMesh();
}

void ACudgelWeapon::SetWeaponMesh()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponMesh(
        TEXT("/Script/Engine.StaticMesh'/Game/Fantasy_Pack/Characters/Troll/Mesh/SM_Cudgel.SM_Cudgel'"));

    checkf(WeaponMesh.Succeeded(), TEXT("Find Weapon Mesh not Succeeded"))

        WeaponStaticMesh->SetStaticMesh(WeaponMesh.Object);
}
