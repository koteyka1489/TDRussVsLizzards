// TD Russ Vs Lizzards Game

#include "Components/WeaponComponent.h"
#include "Weapon/BaseWeapon.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    Weapon = CreateDefaultSubobject<ABaseWeapon>("ABaseWeapon");
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UWeaponComponent::SetWeaponMesh(TObjectPtr<UStaticMesh> WeaponMeshIn) 
{
    if (Weapon)
    {
        Weapon->SetWeaponMesh(WeaponMeshIn);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Weapon nullptr"));
        checkNoEntry();
    }
}
