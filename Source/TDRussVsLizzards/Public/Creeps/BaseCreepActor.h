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

UCLASS()
class TDRUSSVSLIZZARDS_API ABaseCreepActor : public AActor
{
    GENERATED_BODY()

public:
    ABaseCreepActor();
    virtual void BeginPlay() override;


    TObjectPtr<USkeletalMeshComponent> GetSkeletalMeshComponent() { return SkeletalMeshComponent; }


    void SetCreepIsClicked();
    void SetCreepIsChoisen(bool ChoisenStatus);

    void PlayAnimationIdle();
    void PlayAnimationWalk();
    void PlayAnimationRun();


    FOnCreepIsClicked OnCreepIsClicked;

protected:
    UPROPERTY()
    UCapsuleComponent* CapsuleComponent;

    UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    UPROPERTY()
    UHealthComponent* HealthComponent;

    UPROPERTY()
    UStaticMeshComponent* StaticMeshComponent;


    virtual void InitSkeletalMesh();
    virtual void InitAnimations();

    TObjectPtr<UAnimSequence> CreepIdleAnimation;
    TObjectPtr<UAnimSequence> CreepWalkAnimation;
    TObjectPtr<UAnimSequence> CreepRunAnimation;

private:
    bool bCreepIsChoisen = false;

};
