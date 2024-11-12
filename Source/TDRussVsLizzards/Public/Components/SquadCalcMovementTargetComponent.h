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

    void UpdateNewCreepsPositions(int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor);
    void SetCreepsMovingDestination();
    

    TArray<FVector>& GetNewCreepsLocations() { return NewCreepsLocations; }
    TArray<FVector>& GetNewCreepsLocationsNoRandom() { return NewCreepsLocationsNoRandom; }
    FRotator& GetNewSquadRotation() { return NewSquadRotation; }

protected:
private:
    TObjectPtr<ABaseSquadCreeps> OwnerSquad;
    TArray<TObjectPtr<ABaseCreepActor>>* CreepsArray;
    TArray<FVector> NewCreepsLocations;
    TArray<FVector> NewCreepsLocationsNoRandom;
    FRotator NewSquadRotation;
};
