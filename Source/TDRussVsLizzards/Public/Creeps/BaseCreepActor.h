// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCreepActor.generated.h"

DECLARE_DELEGATE(FOnCreepIsClicked);

class UHealthComponent;
class USkeletalMeshComponent;
class UActorMovementComponent;
class UCapsuleComponent;
class UWeaponComponent;

UCLASS()
class TDRUSSVSLIZZARDS_API ABaseCreepActor : public AActor
{
    GENERATED_BODY()

public:
    ABaseCreepActor();
    virtual void BeginPlay() override;

    TObjectPtr<USkeletalMeshComponent> GetSkeletalMeshComponent() { return SkeletalMeshComponent; }

    void SetCreepIsClicked();
    void SetCreepIsNotClicked();

    FOnCreepIsClicked OnCreepIsClicked;

protected:
    UPROPERTY()
    UCapsuleComponent* CapsuleComponent;

    UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    UPROPERTY()
    UHealthComponent* HealthComponent;

    UPROPERTY()
    UActorMovementComponent* MovementComponent;

    virtual void InitSkeletalMesh();
    virtual void InitAnimations();

    TObjectPtr<UAnimSequence> CreepIdleAnimation;
    TObjectPtr<UAnimSequence> CreepWalkAnimation;
    TObjectPtr<UAnimSequence> CreepRunAnimation;

private:
    
};
