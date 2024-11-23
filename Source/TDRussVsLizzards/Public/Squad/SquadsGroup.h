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
    void AddSquad(const TObjectPtr<ABaseSquadCreeps>& Squad);
    
protected:

    
private:
    TArray<TObjectPtr<ABaseSquadCreeps>> GroupedSquads;

    FVector CenterLocationGroup = FVector::ZeroVector;
    FVector RightFrontCornerGroup = FVector::ZeroVector;
    FVector LeftFrontCornerGroup = FVector::ZeroVector;
    
    void CalculateBaseGroupLocations();
    
};
