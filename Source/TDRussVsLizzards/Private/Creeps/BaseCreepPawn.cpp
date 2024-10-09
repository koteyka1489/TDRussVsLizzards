// TD Russ Vs Lizzards Game

#include "Creeps/BaseCreepPawn.h"

ABaseCreepPawn::ABaseCreepPawn()
{

    PrimaryActorTick.bCanEverTick = true;
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
