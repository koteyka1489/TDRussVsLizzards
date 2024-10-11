// TD Russ Vs Lizzards Game

#include "Squad/BaseSquadCreeps.h"
#include "Creeps/BaseCreepActor.h"

ABaseSquadCreeps::ABaseSquadCreeps()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABaseSquadCreeps::BeginPlay()
{
    Super::BeginPlay();

    Creeps.Reserve(CreepsNum);
    SpawnCreeps();


}

void ABaseSquadCreeps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}




void ABaseSquadCreeps::SpawnCreeps()
{
    int32 CreepsRows    = FMath::FloorToInt(FMath::Sqrt((float)CreepsNum));
    int32 CreepsColumns = CreepsRows;
    int32 CreepsRemainder = CreepsNum - CreepsRows * CreepsColumns;
    FVector Squadlocation = GetActorLocation();
    FRotator SpawnRotation{0.0, 180.0, 0.0};

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 y = 0; y < CreepsRows; y++)
    {
        for (int32 x = 0; x < CreepsColumns; x++)
        {
            
            FVector SpawnLocation =
                FVector(Squadlocation.X + 200.0 * (double)x, Squadlocation.Y + 200.0 * (double)y, Squadlocation.Z + 90.0);

            Creeps.Add(GetWorld()->SpawnActor<ABaseCreepActor>(SpawnLocation, SpawnRotation, SpawnInfo));
        }
    }

    for (int32 y = CreepsRows; y < CreepsRows + 1; y++)
    {
        for (int32 x = 0; x < CreepsRemainder ; x++)
        {
            FVector SpawnLocation =
                FVector(Squadlocation.X + 200.0 * (double)x, Squadlocation.Y + 200.0 * (double)y, Squadlocation.Z + 90.0);

            Creeps.Add(GetWorld()->SpawnActor<ABaseCreepActor>(SpawnLocation, SpawnRotation, SpawnInfo));
        }
    }
}
