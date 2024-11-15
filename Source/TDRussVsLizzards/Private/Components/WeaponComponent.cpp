// TD Russ Vs Lizzards Game

#include "Components/WeaponComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Creeps/BaseCreepActor.h"
#include "SkeletalMeshComponentBudgeted.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!AttachWeaponToSocket())
    {
        UE_LOG(LogTemp, Error, TEXT("Attach Weapon to Socket is failed"));
    }
}

void UWeaponComponent::InitWeaponType(TSubclassOf<ABaseWeapon> WeaponTypeIn)
{
    WeaponType = WeaponTypeIn;
}

bool UWeaponComponent::AttachWeaponToSocket()
{
    auto OwnerActor = Cast<ABaseCreepActor>(GetOwner());
    checkf(IsValid(OwnerActor), TEXT("Cast Actor is failed"));

    auto SkeletalMeshComp = OwnerActor->GetSkeletalMeshComponent();
    checkf(IsValid(SkeletalMeshComp), TEXT("get skeletal mesh is failed"));

    checkf(SkeletalMeshComp->DoesSocketExist(FName("WeaponSocket")), TEXT("Weapon socket  does not exist on the SkeletalMeshComponent"));

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    WeaponInst = GetWorld()->SpawnActor<ABaseWeapon>(WeaponType);
    checkf(IsValid(WeaponInst), TEXT("Spawn Actor is Failed"));

    return WeaponInst->AttachToComponent(SkeletalMeshComp, AttachmentRules, "WeaponSocket");
}
