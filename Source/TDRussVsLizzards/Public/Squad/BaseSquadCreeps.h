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

    void MoveAndRotatingSquadToLocation(FVector Destination);

    void OnCreepIsClicked();

    UActorMovementComponent* GetSquadMovementComponent() { return MovementComponent; }

    TArray<TObjectPtr<ABaseCreepActor>>* GetCreeps() { return &Creeps; }
    FVector GetRightCornerCreepLocation();
    FVector GetLeftCornerCreepLocation();
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

    FSquadSizes CurrentSquadSizes;
    ESquadCurrentAnimation CurrentAnimation = ESquadCurrentAnimation::Idle;

    TQueue<TObjectPtr<USquadBaseTask>> SquadTasksQueue;
    TObjectPtr<USquadBaseTask> CurrentSquadTask = nullptr;
    bool bCurrentSquadTaskIsExecute             = false;
    bool bSquadIsChosen                         = false;




    void UpdateSquadLocationStart();
    void SetBoxExtendBySquadSize();

    void SpawnCreepsN();
    FSquadSizes CalculateCurrentSquadSizes();

    void BindOnCreepIsClickedtDelegate();

    void OnMovingComplete();
    void OnRotatingCreepsComplete();
    void OnRotatingFrontSquadComplete();

    double CalculateDotFrontSquadToLocation(FVector Location);
    double CalculateDotRightVectorSquadToLocation(FVector Location);
    ERotateFrontSquadBySide GetSideToFrontSquadRotating(FVector Location);
    void ExecuteCurrentTaskQueue();
};
