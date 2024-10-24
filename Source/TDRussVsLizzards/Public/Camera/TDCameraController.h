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
DECLARE_DELEGATE_OneParam(FOnRotateCamera, float);
DECLARE_DELEGATE_OneParam(FOnLeftMouseClickChois, FHitResult);
DECLARE_DELEGATE_OneParam(FOnRightMouseClickChois, FHitResult);
DECLARE_DELEGATE_OneParam(FOnChangeAngleCamera, float);
DECLARE_DELEGATE_OneParam(FOnMultiplySelectSquad, bool);
DECLARE_DELEGATE(FOnLeftMouseHold);
DECLARE_DELEGATE(FOnLeftMouseHoldCompleted);

UCLASS()
class TDRUSSVSLIZZARDS_API ATDCameraController : public APlayerController
{
    GENERATED_BODY()

public:
    ATDCameraController();
    FVector GetMouseLocationOnTerrain();

    // Delegates
    FOnZoomChanged OnZoomChanged;
    FOnMoveCameraUpDown OnMoveCameraUpDown;
    FOnMoveCameraRightLeft OnMoveCameraRightLeft;
    FOnRotateCamera OnRotateCamera;
    FOnLeftMouseClickChois OnLeftMouseClickChois;
    FOnRightMouseClickChois OnRightMouseClickChois;
    FOnChangeAngleCamera OnChangeAngleCamera;
    FOnMultiplySelectSquad OnMultiplySelectSquad;
    FOnLeftMouseHold OnLeftMouseHold;
    FOnLeftMouseHoldCompleted OnLeftMouseHoldCompleted;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* RotateRightCameraAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* RotateLeftCameraAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* LeftClickMouse;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* RightClickMouse;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* CameraAngleUpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* CameraAngleDownAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MultiplySelectSquadsAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mouse")
    float LeftMouseButtonHoldTreshold = 0.08;

private:
    void MoveCameraUpDown(const FInputActionValue& Value);
    void MoveCameraRightLeft(const FInputActionValue& Value);
    void ZoomUpAction(const FInputActionValue& Value);
    void RotateCamera(const FInputActionValue& Value);
    void ChangeAngleCamera(const FInputActionValue& Value);
    void SetLeftClickChois();
    void SetRightClickChois();
    void MultiplySelectSquadsOn();
    void MultiplySelectSquadsOff();
    void LeftClickTriggered();
    void LeftClickCompleted();

    FHitResult GetClickHit();
    
    bool bSelectionBoxIsSpawned = false;
    float LefMouseDownTime = 0.0f;
};
