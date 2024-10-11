// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCreepActor.generated.h"

class UHealthComponent;
class USkeletalMeshComponent;
class ATDGoal;
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

private:
    TObjectPtr<ATDGoal> Goal;
    TObjectPtr<UAnimSequence> CreepRunAnimation;
};