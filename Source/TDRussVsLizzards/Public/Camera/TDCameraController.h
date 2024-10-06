// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TDCameraController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DELEGATE_OneParam(FOnZoomChanged, float);
DECLARE_DELEGATE_OneParam(FOnMoveCameraUpDown, float);
DECLARE_DELEGATE_OneParam(FOnMoveCameraRightLeft, float);

UCLASS()
class TDRUSSVSLIZZARDS_API ATDCameraController : public APlayerController
{
    GENERATED_BODY()

public:
    ATDCameraController();
    FOnZoomChanged OnZoomChanged;
    FOnMoveCameraUpDown OnMoveCameraUpDown;
    FOnMoveCameraRightLeft OnMoveCameraRightLeft;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ZoomUpCameraAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ZoomDownCameraAction;

    

private:
    void MoveCameraUpDown(const FInputActionValue& Value);
    void MoveCameraRightLeft(const FInputActionValue& Value);
    void ZoomUpAction(const FInputActionValue& Value);


};
