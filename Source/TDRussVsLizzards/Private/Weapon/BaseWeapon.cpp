// TD Russ Vs Lizzards Game


#include "Weapon/BaseWeapon.h"
#include "Engine/StaticMesh.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("UStaticMeshComponent");
    WeaponStaticMesh->SetupAttachment(GetRootComponent());

}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWeapon::SetWeaponMesh(TObjectPtr<UStaticMesh> WeaponMeshIn) 
{
    if (WeaponStaticMesh)
	{
        WeaponStaticMesh->SetStaticMesh(WeaponMeshIn);
	}
	else
	{
        UE_LOG(LogTemp, Error, TEXT("WeaponMesh nullptr"));
        checkNoEntry();
	}

	
}
