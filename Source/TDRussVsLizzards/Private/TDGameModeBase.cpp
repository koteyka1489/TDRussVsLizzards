// TD Russ Vs Lizzards Game

#include "TDGameModeBase.h"
#include "Camera/TDCameraController.h"
#include "Camera/CameraPawn.h"
#include "Creeps/BaseCreepActor.h"

ATDGameModeBase::ATDGameModeBase()
{
    DefaultPawnClass      = ACameraPawn::StaticClass();
    PlayerControllerClass = ATDCameraController::StaticClass();
}

void ATDGameModeBase::StartPlay()
{
    Super::StartPlay();
    //SpawnedCreeps.Reserve(GameData.CreepsNum);
    //SpawnCreeps();
}



void ATDGameModeBase::SpawnCreeps()
{

    int32 CreepsRows    = FMath::FloorToInt(FMath::Sqrt((float)GameData.CreepsNum));
    int32 CreepsColumns = CreepsRows;

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 y = 0; y < CreepsRows; y++)
    {
        for (int32 x = 0; x < CreepsColumns; x++)
        {
            FRotator SpawnRotation = FRotator::ZeroRotator;
            FVector SpawnLocation  = FVector(200.0 * (double)x, 200.0 * (double)y, 90.0);

            SpawnedCreeps.Add(GetWorld()->SpawnActor<ABaseCreepActor>(SpawnLocation, SpawnRotation, SpawnInfo));
        }
    }
}
