// TD Russ Vs Lizzards Game

#include "TDGameModeBase.h"
#include "Camera/TDCameraController.h"
#include "Camera/CameraPawn.h"
#include "Creeps/BaseCreepPawn.h"
#include "AI/TDAIController.h"

ATDGameModeBase::ATDGameModeBase()
{
    DefaultPawnClass      = ACameraPawn::StaticClass();
    PlayerControllerClass = ATDCameraController::StaticClass();
}

void ATDGameModeBase::StartPlay()
{
    Super::StartPlay();
    SpawnCreeps();
}

UClass* ATDGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return CreepPawn;
    }


    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ATDGameModeBase::SpawnCreeps()
{
    int32 CreepsRows = GameData.CreepsNum / CreepsInRow;
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


    for (int32 y = 0; y < CreepsRows; y++)
    {
        for (int32 x = 0; x < CreepsInRow; x++)
        {
            FRotator BaseRotation = FRotator::ZeroRotator;
            FVector BaseLocation  = FVector(50.0 * (double)x, 50.0 * (double)y, 0.0);

            const auto CreepAiController = GetWorld()->SpawnActor<AAIController>(CreepController, SpawnInfo);
            RestartPlayer(CreepAiController);
        }
    }
}
