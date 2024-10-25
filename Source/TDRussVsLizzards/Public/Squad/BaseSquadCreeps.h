// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSquadCreeps.generated.h"

DECLARE_DELEGATE_OneParam(FOnSquadIsChoisen, ABaseSquadCreeps*);
DECLARE_DELEGATE_OneParam(FOnSquadIsUnChoisen, ABaseSquadCreeps*);

class ABaseCreepActor;
class UBoxComponent;
class UActorMovementComponent;
class USceneComponent;



struct FSquadSizes
{
    int32 Width;
    int32 Heigth;
};

enum class ESquadMovingDirection
{
    FrontMoving,
    BackMoving,
    LeftMoving,
    RightMoving
};

enum class ESquadCurrentAnimation
{
    Idle,
    Walk,
    Run
};

UCLASS()
class TDRUSSVSLIZZARDS_API ABaseSquadCreeps : public AActor
{
    GENERATED_BODY()

public:
    ABaseSquadCreeps();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void SquadUnChoisen();
    void SquadUnChoisenBySelectBox();

    void MoveToLocation(FVector Destination);
    void RotateFrontSquadToLocation(FVector Destination);
    void OnCreepIsClicked();

    TArray<TObjectPtr<ABaseCreepActor>>* GetCreeps() { return &Creeps; }

    FOnSquadIsChoisen OnSquadIsChoisen;
    FOnSquadIsUnChoisen OnSquadIsUnChoisen;

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UActorMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UBoxComponent* SquadSizesBox;



    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    int32 CreepsNum = 50;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    double CreepPositionRandom = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    TSubclassOf<ABaseCreepActor> CreepsType;

   


private:
    TArray<TObjectPtr<ABaseCreepActor>> Creeps;
    TArray<FVector> CreepsLocationFromCenterSquad;

    bool bSquadIsChosen = false;
    FSquadSizes CurrentSquadSizes;

    ESquadMovingDirection CurrentMovingDirection = ESquadMovingDirection::FrontMoving;
    ESquadCurrentAnimation CurrentAnimation      = ESquadCurrentAnimation::Idle;

    void UpdateSquadLocationStart();
    void UpdateCreepsLocationFromCenterSquad();
    void SetBoxExtendBySquadSize();
    
    ESquadMovingDirection CalculateSquadMovingDirection(FVector Destination);

    void SpawnCreepsN();
    FSquadSizes CalculateCurrentSquadSizes();

    void BindOnCreepIsClickedtDelegate();

    void OnMovingComplete();
};
