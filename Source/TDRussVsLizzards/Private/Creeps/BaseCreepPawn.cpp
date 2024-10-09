// TD Russ Vs Lizzards Game

#include "Creeps/BaseCreepPawn.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Goal/TDGoal.h"
#include "Components/TDPawnMovementComponent.h"

ABaseCreepPawn::ABaseCreepPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SetRootComponent(SkeletalMeshComponent);

    HealthComponent         = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
    TDPawnMovementComponent = CreateDefaultSubobject<UTDPawnMovementComponent>("TDPawnMovementComponent");
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

    TDPawnMovementComponent->MoveToLocation(Goal->GetActorLocation());
}

void ABaseCreepPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseCreepPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
