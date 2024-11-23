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
DECLARE_DELEGATE(FOnRightMouseHold);
DECLARE_DELEGATE(FOnRightMouseHoldCompleted);
DECLARE_DELEGATE_OneParam(FOnChangeAngleCamera, float);
DECLARE_DELEGATE_OneParam(FOnMultiplySelectSquad, bool);
DECLARE_DELEGATE(FOnLeftMouseHold);
DECLARE_DELEGATE(FOnLeftMouseHoldCompleted);
DECLARE_DELEGATE(FOnStopSquad);
DECLARE_DELEGATE(FOnGroupingSquad);

UCLASS()
class TDRUSSVSLIZZARDS_API ATDCameraController : public APlayerController
{
    GENERATED_BODY()

public:
    ATDCameraController();
    FVector GetMouseLocationOnTerrain();

    // Delegates Camera Pawn
    FOnZoomChanged OnZoomChanged;
    FOnMoveCameraUpDown OnMoveCameraUpDown;
    FOnMoveCameraRightLeft OnMoveCameraRightLeft;
    FOnRotateCamera OnRotateCamera;
    FOnChangeAngleCamera OnChangeAngleCamera;

    // Delegates Team Controller
    FOnLeftMouseClickChois OnLeftMouseClickChois;
    FOnRightMouseClickChois OnRightMouseClickChois;
    FOnMultiplySelectSquad OnMultiplySelectSquad;
    FOnLeftMouseHold OnLeftMouseHold;
    FOnLeftMouseHoldCompleted OnLeftMouseHoldCompleted;
    FOnStopSquad OnStopSquad;
    FOnRightMouseHold OnRightMouseHold;
    FOnRightMouseHoldCompleted OnRightMouseHoldCompleted;
    FOnGroupingSquad OnGroupingSquad;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* StopSquadAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* GroupingSquadAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mouse")
    float LeftMouseButtonHoldTreshold = 0.08f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mouse")
    float RightMouseButtonHoldTreshold = 0.15f;

private:
    void MoveCameraUpDown(const FInputActionValue& Value);
    void MoveCameraRightLeft(const FInputActionValue& Value);
    void ZoomUpAction(const FInputActionValue& Value);
    void RotateCamera(const FInputActionValue& Value);
    void ChangeAngleCamera(const FInputActionValue& Value);

    void MultiplySelectSquadsOn();
    void MultiplySelectSquadsOff();

    void StopSquad();
    void GroupingSquad();

    void LeftClickStarted();
    void LeftClickTriggered();
    void LeftClickCompleted();
    
    void RightClickTriggered();
    void RightClickCompleted();

    FHitResult GetClickHit();

    bool bSelectionBoxIsSpawned = false;
    bool bRebuildSquadIsContinue = false;
    float LeftMouseDownTime     = 0.0f;
    float RightMouseDownTime    = 0.0f;

};
