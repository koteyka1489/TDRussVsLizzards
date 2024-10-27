// TD Russ Vs Lizzards Game


#include "Squad/Task/MoveSquadTask.h"
#include "Squad/BaseSquadCreeps.h"
#include "Components/ActorMovementComponent.h"

void UMoveSquadTask::ExecuteTask() 
{
    SquadOwner->GetSquadMovementComponent()->MoveToLocation(Destination);
}
