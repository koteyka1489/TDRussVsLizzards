// TD Russ Vs Lizzards Game

#include "Components/WeaponComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Creeps/BaseCreepActor.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    WeaponType                        = ABaseWeapon::StaticClass();
}

void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AttachWeaponToSocket())
    {}
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Attach Weapon is failed"));
        checkNoEntry();
    }
}

bool UWeaponComponent::AttachWeaponToSocket()
{
    auto OwnerActor = Cast<ABaseCreepActor>(GetOwner());
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Cast Actor failed"));
        checkNoEntry();
    }

    auto SkeletalMeshComp = OwnerActor->GetSkeletalMeshComponent();
    if (!SkeletalMeshComp)
    {
        UE_LOG(LogTemp, Error, TEXT(" get skeletal mesh is failed"));
        checkNoEntry();
    }

    if (!SkeletalMeshComp->DoesSocketExist(FName("WeaponSocket")))
    {
        UE_LOG(LogTemp, Error, TEXT("Weapon socket  does not exist on the SkeletalMeshComponent"));
        checkNoEntry();
    }

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponType);
    if (Weapon)
    {
        bool AttachResult = Weapon->AttachToComponent(SkeletalMeshComp, AttachmentRules, "WeaponSocket");
        FString Message   = FString::Printf(TEXT("Attachment result is - %s %s"), AttachResult ? TEXT("TRUE") : TEXT("FALSE"), *GetName());
        GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, Message);
        return AttachResult;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Spawn Actor is Failed"));
        checkNoEntry();
    }

    return false;
}
