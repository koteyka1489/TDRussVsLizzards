// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCreepActor.generated.h"

class UHealthComponent;
class USkeletalMeshComponent;
class UActorMovementComponent;
class USceneComponent;

UCLASS()
class TDRUSSVSLIZZARDS_API ABaseCreepActor : public AActor
{
    GENERATED_BODY()

public:
    ABaseCreepActor();

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    USceneComponent* SceneComponent;

    UPROPERTY()
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    UPROPERTY()
    UHealthComponent* HealthComponent;

    UPROPERTY()
    UActorMovementComponent* MovementComponent;

    virtual  void InitSkeletalMesh();
    virtual  void InitAnimations();

    TObjectPtr<UAnimSequence> CreepIdleAnimation;
    TObjectPtr<UAnimSequence> CreepWalkAnimation;
    TObjectPtr<UAnimSequence> CreepRunAnimation;

    
};
