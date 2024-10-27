// TD Russ Vs Lizzards Game


#include "Squad/Task/SquadBaseTask.h"
#include "Squad/BaseSquadCreeps.h"


void USquadBaseTask::InitDestinationTask(FVector DestinationIn, ABaseSquadCreeps* Owner)
{
    Destination = DestinationIn;
    SquadOwner  = Owner;
}

void USquadBaseTask::ExecuteTask() {}
