// TD Russ Vs Lizzards Game

#include "Creeps/BaseCreepPawn.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Goal/TDGoal.h"
#include "AI/TDAIController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"

ABaseCreepPawn::ABaseCreepPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    SetRootComponent(CapsuleComponent);

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
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
}

void ABaseCreepPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector Direction = (Goal->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    AddMovementInput(Direction, 1.0f);
}

void ABaseCreepPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
