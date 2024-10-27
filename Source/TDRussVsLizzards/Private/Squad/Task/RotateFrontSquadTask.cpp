// TD Russ Vs Lizzards Game


#include "Squad/Task/RotateFrontSquadTask.h"
#include "Squad/BaseSquadCreeps.h"
#include "Components/ActorMovementComponent.h"

void URotateFrontSquadTask::ExecuteTask() 
{
    SquadOwner->GetSquadMovementComponent()->RotateFrontSquadToLocation(Destination);
}
