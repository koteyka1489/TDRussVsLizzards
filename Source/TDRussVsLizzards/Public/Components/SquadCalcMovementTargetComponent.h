// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SquadCalcMovementTargetComponent.generated.h"

class ABaseSquadCreeps;
class ABaseCreepActor;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API USquadCalcMovementTargetComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USquadCalcMovementTargetComponent();
    virtual void BeginPlay() override;

    void MoveToLocation(FVector Destination);

protected:
private:
    // Fields
    TObjectPtr<ABaseSquadCreeps> OwnerSquad;
    TArray<TObjectPtr<ABaseCreepActor>>* CreepsArray;

    // Methods

};
