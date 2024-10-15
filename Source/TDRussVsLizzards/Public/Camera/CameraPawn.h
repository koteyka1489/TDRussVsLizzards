// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USceneComponent;

UCLASS()
class TDRUSSVSLIZZARDS_API ACameraPawn : public APawn
{
    GENERATED_BODY()

public:
    ACameraPawn();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USceneComponent* SceneComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float ZoomStart = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float ZoomMin = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float ZoomMax = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float SpeedZoom = 3000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
    float SpeedCamera = 3000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
    float SpeedRotateCamera = 100.0f;

private:
    void OnZoomChanged(float Direction);
    void OnMoveCameraUpDown(float Direction);
    void OnMoveCameraRightLeft(float Direction);
    void OnRotateCamera(float Direction);
    void OnLeftMouseClickChois(FHitResult Hit);

};
