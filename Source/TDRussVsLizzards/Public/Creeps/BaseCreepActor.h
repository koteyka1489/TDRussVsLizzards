// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCreepActor.generated.h"

class UHealthComponent;
class USkeletalMeshComponent;
class UActorMovementComponent;
class USceneComponent;
class UWeaponComponent;

UCLASS()
class TDRUSSVSLIZZARDS_API ABaseCreepActor : public AActor
{
    GENERATED_BODY()

public:
    ABaseCreepActor();
    virtual void BeginPlay() override;

protected:
    UPROPERTY()
    USceneComponent* SceneComponent;

    UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    UPROPERTY()
    UHealthComponent* HealthComponent;

    UPROPERTY()
    UActorMovementComponent* MovementComponent;

    UPROPERTY()
    UWeaponComponent* WeaponComponent;

    virtual void InitSkeletalMesh();
    virtual void InitAnimations();
    virtual void InitWeapon();

    TObjectPtr<UAnimSequence> CreepIdleAnimation;
    TObjectPtr<UAnimSequence> CreepWalkAnimation;
    TObjectPtr<UAnimSequence> CreepRunAnimation;

    private:
    void AttachWeaponToSocket();
};
