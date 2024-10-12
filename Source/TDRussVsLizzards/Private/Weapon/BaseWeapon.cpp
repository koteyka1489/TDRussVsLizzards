// TD Russ Vs Lizzards Game


#include "Weapon/BaseWeapon.h"
#include "Engine/StaticMesh.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMesh>("WeaponMesh");


}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
