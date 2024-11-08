// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCreepActor.generated.h"

class UHealthComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UWeaponComponent;
class UStaticMeshComponent;
class UAnimMontage;
class USkeletalMeshComponentBudgeted;
class USceneComponent;

USTRUCT(BlueprintType)
struct FCreepSpeeds
{
    GENERATED_BODY()

    float SpeedMoving   = 100.0f;
    float SpeedRotating = 20.0f;
};

UCLASS(Abstract)
class TDRUSSVSLIZZARDS_API ABaseCreepActor : public AActor
{
    GENERATED_BODY()

public:
    ABaseCreepActor();
    virtual void BeginPlay() override;

    TObjectPtr<USkeletalMeshComponentBudgeted> GetSkeletalMeshComponent() { return SkeletalMeshComponent; }

    void SetCreepIsClicked();
    void SetCreepIsChoisen(bool ChoisenStatus);


    void SetCreepSpeeds(float SpeedRotatingIn, float SpeedMovingIn);

    UFUNCTION(BlueprintCallable, Category = "Speed")
    float GetCreepCurrentSpeedMoving() { return CreepCurrentSpeeds.SpeedMoving; }

    FVector& GetMovingDestination() { return MovingDestination; }

    void SetMovingDestination(FVector MovingDestinationIn) { MovingDestination = MovingDestinationIn; }



protected:
    UPROPERTY()
    TObjectPtr<USceneComponent> SceneComponent;

    UPROPERTY()
    TObjectPtr<USkeletalMeshComponentBudgeted> SkeletalMeshComponent;

    UPROPERTY()
    TObjectPtr<UHealthComponent> HealthComponent;

    UPROPERTY()
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY()
    TObjectPtr<UAnimSequence> CreepIdleAnimation;

    UPROPERTY()
    TObjectPtr<UAnimSequence> CreepWalkAnimation;

    UPROPERTY()
    TObjectPtr<UAnimSequence> CreepRunAnimation;

    virtual void InitSkeletalMesh();
    virtual void InitAnimations();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
     FCreepSpeeds CreepMaxSpeeds;

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
     FCreepSpeeds CreepCurrentSpeeds;

private:
    bool bCreepIsChoisen = false;

    FVector MovingDestination = FVector::Zero();

   
};
