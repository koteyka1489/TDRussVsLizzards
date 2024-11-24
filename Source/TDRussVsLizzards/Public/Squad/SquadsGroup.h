// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SquadsGroup.generated.h"

class ABaseSquadCreeps;

UCLASS()
class TDRUSSVSLIZZARDS_API USquadsGroup : public UObject
{
	GENERATED_BODY()

public:
    USquadsGroup();
    void AddSquad(const TArray<TObjectPtr<ABaseSquadCreeps>>& ChoisenSquads);
    TArray<TObjectPtr<ABaseSquadCreeps>>& GetSquads() { return GroupedSquads; }
    TArray<FVector>& GetSquadGroupRightCornerLocationsFromCenter() {return SquadGroupRightCornerLocationsFromCenter;};
    TArray<FVector>& GetSquadGroupRotationsFromCenter() {return SquadGroupRotationsFromCenter;}; 

    
private:
    TArray<TObjectPtr<ABaseSquadCreeps>> GroupedSquads;
    FVector CenterLocationGroup = FVector::ZeroVector;
    TArray<FVector> SquadGroupRightCornerLocationsFromCenter;
    TArray<FVector> SquadGroupRotationsFromCenter; 
    
    void CalculateBaseGroupLocations();
    void CalculateSquadRotationsAndLocationsFromCenter();
    
};
