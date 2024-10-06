// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDCameraController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DELEGATE_OneParam(FOnZoomChanged, float);

UCLASS()
class TDRUSSVSLIZZARDS_API ATDCameraController : public APlayerController
{
    GENERATED_BODY()

public:
    ATDCameraController();
    FOnZoomChanged OnZoomChanged;

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UPROPERTY()
    APawn* CameraPawn;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ZoomUpCameraAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ZoomDownCameraAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    float SpeedCamera = 3000.0f;

private:
    void MoveCameraUpDown(const FInputActionValue& Value);
    void MoveCameraRightLeft(const FInputActionValue& Value);
    void ZoomUpAction(const FInputActionValue& Value);


};
