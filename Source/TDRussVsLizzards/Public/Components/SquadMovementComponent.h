// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SquadMovementComponent.generated.h"

class ABaseCreepActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API USquadMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USquadMovementComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void SetSquadMovement();
    void StopAllMovings();

protected:
private:
    TArray<TObjectPtr<ABaseCreepActor>>* CreepsArray;
    bool bRotatingSquadIsContinue = false;
    bool bMovingSquadIsContinue   = false;

    void RotatingCreeps(float DeltaTime);
    void MovingCreeps(float DeltaTime);
};
