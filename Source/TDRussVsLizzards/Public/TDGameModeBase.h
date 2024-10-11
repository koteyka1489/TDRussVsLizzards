// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDGameModeBase.generated.h"

class ABaseCreepPawn;
class ABaseCreepActor;

USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Creeps", meta = (ClampMin = "5", ClampMax = "5000"))
    int32 CreepsNum = 1000;
};

UCLASS()
class TDRUSSVSLIZZARDS_API ATDGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATDGameModeBase();
    virtual void StartPlay() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnCreeps")
    FGameData GameData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnCreeps")
    TSubclassOf<ABaseCreepActor> CreepActor;

private:
    TArray<TObjectPtr<ABaseCreepActor>> SpawnedCreeps;

    void SpawnCreeps();
};
