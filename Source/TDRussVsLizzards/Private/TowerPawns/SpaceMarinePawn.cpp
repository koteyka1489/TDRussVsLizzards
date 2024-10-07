// TD Russ Vs Lizzards Game

#include "TowerPawns/SpaceMarinePawn.h"

ASpaceMarinePawn::ASpaceMarinePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SetRootComponent(SkeletalMeshComponent);
}

void ASpaceMarinePawn::BeginPlay()
{
    Super::BeginPlay();
}

void ASpaceMarinePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASpaceMarinePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
