// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseCreepPawn.generated.h"

class UHealthComponent;
class USkeletalMeshComponent;
class ATDGoal;
class UTDPawnMovementComponent;
class USceneComponent;

UCLASS()
class TDRUSSVSLIZZARDS_API ABaseCreepPawn : public APawn
{
    GENERATED_BODY()

public:
    ABaseCreepPawn();
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UHealthComponent* HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UTDPawnMovementComponent* TDPawnMovementComponent;

private:
    TObjectPtr<ATDGoal> Goal;
    TObjectPtr<UAnimSequence> CreepRunAnimation;
};
