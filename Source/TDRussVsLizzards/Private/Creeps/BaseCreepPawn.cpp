// TD Russ Vs Lizzards Game

#include "Creeps/BaseCreepPawn.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Goal/TDGoal.h"
#include "Components/TDPawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "AI/TDAIController.h"

ABaseCreepPawn::ABaseCreepPawn()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SkeletalMeshComponent->SetupAttachment(SceneComponent);

    HealthComponent         = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
    TDPawnMovementComponent = CreateDefaultSubobject<UTDPawnMovementComponent>("TDPawnMovementComponent");

    // Init Pawn
    AutoPossessAI     = EAutoPossessAI::Disabled;
    AIControllerClass = nullptr;

    SkeletalMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
    SkeletalMeshComponent->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
    SkeletalMeshComponent->SetSimulatePhysics(false);
    SkeletalMeshComponent->SetEnableGravity(false);
    SkeletalMeshComponent->bDisableClothSimulation            = true;
    SkeletalMeshComponent->bAllowClothActors                  = false;
    SkeletalMeshComponent->bIgnoreRadialForce                 = true;
    SkeletalMeshComponent->bIgnoreRadialImpulse               = true;
    SkeletalMeshComponent->bReplicatePhysicsToAutonomousProxy = false;
    SkeletalMeshComponent->CastShadow                         = 0u;
    SkeletalMeshComponent->IndirectLightingCacheQuality       = EIndirectLightingCacheQuality::ILCQ_Off;
    SkeletalMeshComponent->HLODBatchingPolicy                 = EHLODBatchingPolicy::Instancing;
    SkeletalMeshComponent->bVisibleInRayTracing               = false;
    SkeletalMeshComponent->bVisibleInRealTimeSkyCaptures      = false;
    SkeletalMeshComponent->bVisibleInReflectionCaptures       = false;
    SkeletalMeshComponent->bEnablePhysicsOnDedicatedServer    = false;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CreepMesh(
        TEXT("/Script/Engine.SkeletalMesh'/Game/Fantasy_Pack/Characters/Orc_Hummer/Mesh/SK_Orc_Hummer.SK_Orc_Hummer'"));
    if (CreepMesh.Succeeded())
    {
        SkeletalMeshComponent->SetSkeletalMesh(CreepMesh.Object.Get());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Wrong Reference of Skeletal Mesh"));
        checkNoEntry();
    }

    static ConstructorHelpers::FObjectFinder<UAnimSequence> CreepRunAnimRef(
        TEXT("/Script/Engine.AnimSequence'/Game/Fantasy_Pack/Characters/Orc_Hummer/Animations/Anim_Orc_Hummer_Run.Anim_Orc_Hummer_Run'"));
    if (CreepRunAnimRef.Succeeded())
    {
        CreepRunAnimation = CreepRunAnimRef.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Wrong Reference of Animation"));
        checkNoEntry();
    }
}

void ABaseCreepPawn::BeginPlay()
{
    Super::BeginPlay();

    Goal = Cast<ATDGoal>(UGameplayStatics::GetActorOfClass(GetWorld(), ATDGoal::StaticClass()));
    if (!Goal)
    {
        UE_LOG(LogTemp, Error, TEXT("Goal Cast Failed"));
        checkNoEntry();
    }
    SkeletalMeshComponent->PlayAnimation(CreepRunAnimation, true);
    TDPawnMovementComponent->MoveToLocation(Goal->GetActorLocation());
}

void ABaseCreepPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
