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

    void PlayAnimationIdle();
    void PlayAnimationWalk();
    void PlayAnimationRun();


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

private:
    bool bCreepIsChoisen = false;
};
