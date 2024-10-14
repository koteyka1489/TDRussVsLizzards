// TD Russ Vs Lizzards Game

#include "Creeps/BaseCreepActor.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorMovementComponent.h"
#include "Components/WeaponComponent.h"
#include "Weapon/BaseWeapon.h"

ABaseCreepActor::ABaseCreepActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SkeletalMeshComponent->SetupAttachment(SceneComponent);

    HealthComponent   = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
    MovementComponent = CreateDefaultSubobject<UActorMovementComponent>("MovementComponent");
    WeaponComponent   = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");


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
    SkeletalMeshComponent->PlayAnimation(CreepIdleAnimation, true);
    
}

void ABaseCreepActor::InitSkeletalMesh() {}

void ABaseCreepActor::InitAnimations() {}



