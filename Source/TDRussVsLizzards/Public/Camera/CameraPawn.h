// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USceneComponent;
class ABaseSquadCreeps;

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
    double ZoomStart = 3000.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    double ZoomMin = 200.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    double ZoomMax = 5000.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    double SpeedZoom = 3000.0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
    double SpeedCamera = 3000.0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
    double SpeedRotateCamera = 100.0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
    double MaxPitchCamera = 40.0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
    double MinPitchCamera = -25.0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Camera")
    double SpeedRotatePitchCamera = 20.0;


private:
    void OnZoomChanged(float Direction);
    void OnMoveCameraUpDown(float Direction);
    void OnMoveCameraRightLeft(float Direction);
    void OnRotateCamera(float Direction);
    void OnChangeAngleCamera(float Direction);
    void OnLeftMouseClickChois(FHitResult Hit);
    void OnRightMouseClickChois(FHitResult Hit);
    void OnSquadIsChoisen(ABaseSquadCreeps* SquadIn);

    void GetSquadsOnLevel();
    void BindOnSquadIsChoisenDelegate();


    TArray<TObjectPtr<ABaseSquadCreeps>> SquadsOnLevel;
    TArray<TObjectPtr<ABaseSquadCreeps>> ChoisenSquads;

};
