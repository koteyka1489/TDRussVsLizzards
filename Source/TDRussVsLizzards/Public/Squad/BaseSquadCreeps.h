// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSquadCreeps.generated.h"

DECLARE_DELEGATE_OneParam(FOnSquadIsChoisen, ABaseSquadCreeps*)


class ABaseCreepActor;

UCLASS()
class TDRUSSVSLIZZARDS_API ABaseSquadCreeps : public AActor
{
    GENERATED_BODY()

public:
    ABaseSquadCreeps();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    void SquadUnChoisen();

    FOnSquadIsChoisen OnSquadIsChoisen;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Squad")
    int32 CreepsNum = 50;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    TSubclassOf<ABaseCreepActor> CreepsType;

private:
    TArray<TObjectPtr<ABaseCreepActor>> Creeps;
    bool bSquadIsChosen = false;


    void SpawnCreeps();
    void BindOnCreepIsClickedtDelegate();

    void OnCreepIsClicked();
    
};
