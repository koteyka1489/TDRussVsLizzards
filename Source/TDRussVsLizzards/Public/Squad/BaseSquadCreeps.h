// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSquadCreeps.generated.h"

DECLARE_DELEGATE_OneParam(FOnSquadIsUnChoisenBySelectionBox, ABaseSquadCreeps*);

class ABaseCreepActor;
class UBoxComponent;
class UActorMovementComponent;
class USceneComponent;
class USquadBaseTask;
enum class ERotateFrontSquadBySide;

struct FSquadSizes
{
    int32 Width;
    int32 Heigth;
};

enum class ESquadCurrentAnimation
{
    Idle,
    Walk,
    Run
};

USTRUCT(BlueprintType)
struct FCreepsOffsetInSquad
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    double X = 200.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    double Y = 200.0;
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
    void PlayRunAnimation();
    void StopAllTasks();

    void MoveAndRotatingSquadToLocation(FVector Destination);

    void SetSquadIsChoisen();

    int32 GetCreepsNum() { return CreepsNum; }
    FCreepsOffsetInSquad GetCreepsOffsetInSquad() { return CreepsOffsetInSquad; }
    double GetCreepPositionRandom() { return CreepPositionRandom; }

    UActorMovementComponent* GetSquadMovementComponent() { return MovementComponent; }

    TArray<TObjectPtr<ABaseCreepActor>>* GetCreeps() { return &Creeps; }
    FVector GetRightCornerCreepLocation();
    FVector GetLeftCornerCreepLocation();

    FOnSquadIsUnChoisenBySelectionBox OnSquadIsUnChoisenBySelectionBox;

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
    double CreepPositionRandom = 20.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    TSubclassOf<ABaseCreepActor> CreepsType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    FCreepsOffsetInSquad CreepsOffsetInSquad;

private:
    TArray<TObjectPtr<ABaseCreepActor>> Creeps;

    FSquadSizes CurrentSquadSizes;
    ESquadCurrentAnimation CurrentAnimation = ESquadCurrentAnimation::Idle;

    TQueue<TObjectPtr<USquadBaseTask>> SquadTasksQueue;
    TObjectPtr<USquadBaseTask> CurrentSquadTask = nullptr;
    bool bCurrentSquadTaskIsExecute             = false;
    bool bSquadIsChosen                         = false;

    void UpdateSquadLocationStart();
    void SetBoxExtendBySquadSize();

    void SpawnCreeps();
    TArray<FVector> CalculateCreepsPositions(int32 HeightStart, int32 HeightEnd, int32 WidthStart, int32 WidthEnd, FVector SquadBaseSpawnLocation);
    FSquadSizes CalculateCurrentSquadSizes();

    void OnMovingComplete();
    void OnRotatingCreepsComplete();
    void OnRotatingFrontSquadComplete();

    double CalculateDotFrontSquadToLocation(FVector Location);
    double CalculateDotRightVectorSquadToLocation(FVector Location);
    ERotateFrontSquadBySide GetSideToFrontSquadRotating(FVector Location);
    void ExecuteCurrentTaskQueue();
};
