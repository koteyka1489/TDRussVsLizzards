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
class UCreepMovementComponent;

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

    void SetCreepMaxSpeeds(float SpeedRotatingIn, float SpeedMovingIn);

    bool TickCreepMoving(float& DeltaTime);
    bool TickCreepRotating(float& DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Speed")
    float GetCreepCurrentSpeedMoving();

    UFUNCTION(BlueprintCallable, Category = "Speed")
    float GetCreepCurrentSpeedRotating();

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
    TObjectPtr<UCreepMovementComponent> MovementComponent;

    virtual void InitSkeletalMesh();
    virtual void InitAnimations();

private:
    bool bCreepIsChoisen = false;
};
