// TD Russ Vs Lizzards Game

#include "Components/WeaponComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Creeps/BaseCreepActor.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    WeaponType = ABaseWeapon::StaticClass();
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(AttachWeaponToSocket(), TEXT("Attach Weapon to is failed"));
}

bool UWeaponComponent::AttachWeaponToSocket()
{
    auto OwnerActor = Cast<ABaseCreepActor>(GetOwner());
    checkf(IsValid(OwnerActor), TEXT("Cast Actor is failed"));

    auto SkeletalMeshComp = OwnerActor->GetSkeletalMeshComponent();
    checkf(IsValid(SkeletalMeshComp), TEXT("get skeletal mesh is failed"));

    checkf(SkeletalMeshComp->DoesSocketExist(FName("WeaponSocket")), TEXT("Weapon socket  does not exist on the SkeletalMeshComponent"));

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponType);
    checkf(IsValid(Weapon), TEXT("Spawn Actor is Failed"));

    return Weapon->AttachToComponent(SkeletalMeshComp, AttachmentRules, "WeaponSocket");
}
