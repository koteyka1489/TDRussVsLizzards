// TD Russ Vs Lizzards Game

#include "Creeps/BaseCreepPawn.h"
#include "Components/HealthComponent.h"

ABaseCreepPawn::ABaseCreepPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SetRootComponent(SkeletalMeshComponent);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

}

void ABaseCreepPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseCreepPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseCreepPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
