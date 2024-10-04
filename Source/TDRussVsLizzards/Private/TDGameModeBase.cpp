// TD Russ Vs Lizzards Game


#include "TDGameModeBase.h"
#include "Camera/TDCameraController.h"
#include "Camera/CameraPawn.h"

ATDGameModeBase::ATDGameModeBase() 
{
    DefaultPawnClass = ACameraPawn::StaticClass();
    PlayerControllerClass = ATDCameraController::StaticClass();
}
