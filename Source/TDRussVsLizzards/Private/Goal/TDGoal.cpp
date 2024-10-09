// TD Russ Vs Lizzards Game

#include "Goal/TDGoal.h"

ATDGoal::ATDGoal()
{
    PrimaryActorTick.bCanEverTick = false;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    SetRootComponent(StaticMesh);
}

void ATDGoal::BeginPlay()
{
    Super::BeginPlay();
}

