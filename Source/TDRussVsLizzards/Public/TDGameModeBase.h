// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TDGameModeBase.generated.h"

class ABaseCreepPawn;
class AAIController;


USTRUCT(BlueprintType)
struct FGameData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Creeps", meta = (ClampMin = "5", ClampMax = "1000"))
    int32 CreepsNum = 50;
};

UCLASS()
class TDRUSSVSLIZZARDS_API ATDGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATDGameModeBase();
    virtual void StartPlay() override;
    virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnCreeps")
    FGameData GameData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnCreeps")
    TSubclassOf<AAIController> CreepController;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnCreeps")
    TSubclassOf<ABaseCreepPawn> CreepPawn;



    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpawnCreeps")
    int32 CreepsInRow = 10;


private:
    TArray<TObjectPtr<ABaseCreepPawn>> SpawnedCrepps;


    void SpawnCreeps();
};
