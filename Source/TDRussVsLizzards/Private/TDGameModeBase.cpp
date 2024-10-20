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

}
