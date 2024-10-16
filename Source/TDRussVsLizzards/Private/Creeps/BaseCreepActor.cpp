// TD Russ Vs Lizzards Game

#include "Creeps/BaseCreepActor.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WeaponComponent.h"
#include "Weapon/BaseWeapon.h"
#include "Components\CapsuleComponent.h"
#include "DrawDebugHelpers.h"

ABaseCreepActor::ABaseCreepActor()
{
    PrimaryActorTick.bCanEverTick = false;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    SetRootComponent(CapsuleComponent);

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

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
}

void ABaseCreepActor::BeginPlay()
{
    Super::BeginPlay();
    check(IsValid(CapsuleComponent));
    check(IsValid(SkeletalMeshComponent));
    check(IsValid(HealthComponent));

    SkeletalMeshComponent->PlayAnimation(CreepIdleAnimation, true);
}

void ABaseCreepActor::SetCreepIsClicked()
{
    checkf(OnCreepIsClicked.ExecuteIfBound(), TEXT("OnCreepIsClicked not bound"));
}

void ABaseCreepActor::InitSkeletalMesh() {}

void ABaseCreepActor::InitAnimations() {}
