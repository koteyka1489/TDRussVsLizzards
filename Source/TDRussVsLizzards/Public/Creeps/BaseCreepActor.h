// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCreepActor.generated.h"

DECLARE_DELEGATE(FOnCreepIsClicked);

class UHealthComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UWeaponComponent;
class UStaticMeshComponent;
class UAnimMontage;
class USkeletalMeshComponentBudgeted;

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

    FOnCreepIsClicked OnCreepIsClicked;

protected:
    UPROPERTY()
    TObjectPtr<UCapsuleComponent> CapsuleComponent;

   /* UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;*/

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
