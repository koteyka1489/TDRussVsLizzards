// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SquadMovementComponent.generated.h"

class ABaseCreepActor;

enum class ESquadState
{
    Idle,
    Rotating,
    Movement
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API USquadMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USquadMovementComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void SetSquadMovement();

protected:
private:
    // Fields
    TArray<TObjectPtr<ABaseCreepActor>>* CreepsArray;

    ESquadState CurrentState = ESquadState::Idle;
    TQueue<ESquadState> TasksQueue;

    // Methods
    void RotatingCreeps(float DeltaTime);
    void MovingCreeps(float DeltaTime);
};
