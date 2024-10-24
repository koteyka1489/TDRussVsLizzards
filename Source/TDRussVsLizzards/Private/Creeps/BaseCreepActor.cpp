// TD Russ Vs Lizzards Game

#include "Creeps/BaseCreepActor.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WeaponComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Components\CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

ABaseCreepActor::ABaseCreepActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    SetRootComponent(CapsuleComponent);

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SkeletalMeshComponent->SetupAttachment(GetRootComponent());

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

    check(IsValid(CapsuleComponent));
    CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    CapsuleComponent->SetCapsuleHalfHeight(120.0f);
    CapsuleComponent->SetCapsuleRadius(80.0f);

    check(IsValid(SkeletalMeshComponent));
    SkeletalMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
    SkeletalMeshComponent->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
    SkeletalMeshComponent->SetSimulatePhysics(false);
    SkeletalMeshComponent->SetEnableGravity(false);
    SkeletalMeshComponent->bDisableClothSimulation            = true;
    SkeletalMeshComponent->bAllowClothActors                  = false;
    SkeletalMeshComponent->bIgnoreRadialForce                 = true;
    SkeletalMeshComponent->bIgnoreRadialImpulse               = true;
    SkeletalMeshComponent->bReplicatePhysicsToAutonomousProxy = false;
    SkeletalMeshComponent->CastShadow                         = 1u;
    SkeletalMeshComponent->IndirectLightingCacheQuality       = EIndirectLightingCacheQuality::ILCQ_Off;
    SkeletalMeshComponent->HLODBatchingPolicy                 = EHLODBatchingPolicy::Instancing;
    SkeletalMeshComponent->bVisibleInRayTracing               = false;
    SkeletalMeshComponent->bVisibleInRealTimeSkyCaptures      = false;
    SkeletalMeshComponent->bVisibleInReflectionCaptures       = false;
    SkeletalMeshComponent->bEnablePhysicsOnDedicatedServer    = false;

    check(IsValid(StaticMeshComponent));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SelectCircleMesh(
        TEXT("/Script/Engine.StaticMesh'/Game/Squads/SM_SelectCircle.SM_SelectCircle'"));

    checkf(SelectCircleMesh.Succeeded(), TEXT("Find SelectCircleMesh is not Succeeded "));
    StaticMeshComponent->SetStaticMesh(SelectCircleMesh.Object);
    StaticMeshComponent->SetVisibility(false);
    StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    StaticMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -80.0));
}

void ABaseCreepActor::BeginPlay()
{
    Super::BeginPlay();
    check(IsValid(CapsuleComponent));
    check(IsValid(SkeletalMeshComponent));
    check(IsValid(HealthComponent));

    PlayAnimationIdle();
}

void ABaseCreepActor::SetCreepIsClicked()
{
    if (OnCreepIsClicked.ExecuteIfBound())
    {
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnCreepIsClicked not bound"));
        checkNoEntry();
    }
}

void ABaseCreepActor::SetCreepIsChoisen(bool ChoisenStatus)
{
    bCreepIsChoisen = ChoisenStatus;
    StaticMeshComponent->SetVisibility(ChoisenStatus);
}

void ABaseCreepActor::PlayAnimationIdle()
{
    SkeletalMeshComponent->PlayAnimation(CreepIdleAnimation, true);
}

void ABaseCreepActor::PlayAnimationWalk()
{
    SkeletalMeshComponent->PlayAnimation(CreepWalkAnimation, true);
}

void ABaseCreepActor::PlayAnimationRun()
{
    SkeletalMeshComponent->PlayAnimation(CreepRunAnimation, true);
}

void ABaseCreepActor::InitSkeletalMesh() {}

void ABaseCreepActor::InitAnimations() {}
