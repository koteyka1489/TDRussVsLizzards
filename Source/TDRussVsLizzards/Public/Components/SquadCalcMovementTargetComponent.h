// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SquadCalcMovementTargetComponent.generated.h"

class ABaseSquadCreeps;
class ABaseCreepActor;
class UCreepArray;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API USquadCalcMovementTargetComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USquadCalcMovementTargetComponent();
    virtual void BeginPlay() override;

    void UpdateNewCreepsPositions(int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor);
    void SetCreepsMovingDestination();
    

    TMap<int32, FVector>& GetNewCreepsLocations() { return NewCreepsLocations; }
    TMap<int32, FVector>& GetNewCreepsLocationsNoRandom() { return NewCreepsLocationsNoRandom; }
    FRotator& GetNewSquadRotation() { return NewSquadRotation; }

protected:
private:
    TObjectPtr<ABaseSquadCreeps> OwnerSquad;
    TObjectPtr<UCreepArray> CreepsArray;

    TMap<int32, FVector> NewCreepsLocations;
    TMap<int32, FVector> NewCreepsLocationsNoRandom;

    FRotator NewSquadRotation;
};
