// TD Russ Vs Lizzards Game

#include "Components/SquadCalcMovementTargetComponent.h"
#include "Creeps/BaseCreepActor.h"
#include "Squad/BaseSquadCreeps.h"

USquadCalcMovementTargetComponent::USquadCalcMovementTargetComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void USquadCalcMovementTargetComponent::BeginPlay()
{
    Super::BeginPlay();

    OwnerSquad = Cast<ABaseSquadCreeps>(GetOwner());
    checkf(IsValid(OwnerSquad), TEXT("Get Owner Squad is Failed"));
    CreepsArray = OwnerSquad->GetCreeps();
}

void USquadCalcMovementTargetComponent::MoveToLocation(FVector Destination) 
{
    
    // set rotate task movement component

    // set dest vectors creeps array

}


