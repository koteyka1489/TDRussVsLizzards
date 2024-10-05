// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDCameraController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class TDRUSSVSLIZZARDS_API ATDCameraController : public APlayerController
{
    GENERATED_BODY()

public:
    ATDCameraController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;


    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveCameraUpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveCameraDownAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveCameraRightAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveCameraLeftAction;


private:
    void MoveCameraUp();
    void MoveCameraDown();
    void MoveCameraRight();
    void MoveCameraLeft();
};
