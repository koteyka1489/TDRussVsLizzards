// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSquadCreeps.generated.h"

DECLARE_DELEGATE_OneParam(FOnSquadIsUnChoisenBySelectionBox, ABaseSquadCreeps*);

class ABaseCreepActor;
class UBoxComponent;
class USceneComponent;
class USquadBaseTask;
class UInstancedStaticMeshComponent;
class USquadMovementComponent;
class USquadCalcMovementTargetComponent;

struct FSquadSizes
{
    int32 Width;
    int32 Heigth;
};



USTRUCT(BlueprintType)
struct FSquadCreepsSpeed
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad|CreepSpeed")
    float SpeedMoving   = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad|CreepSpeed")
    float SpeedRotating = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad|CreepSpeed")
    float MovingRandom  = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad|CreepSpeed")
    float RotatingRandom = 100.0f;
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
    void StopAllTasks();

    void MoveAndRotatingSquadToLocation(FVector Destination);
    void UpdateSquadLocationStart();

    void UpdateRebuildngSquad(int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor);
    void EndRebuildSquad();

    void SetSquadIsChoisen();

    int32 GetCreepsNum() { return CreepsNum; }
    FCreepsOffsetInSquad GetCreepsOffsetInSquad() { return CreepsOffsetInSquad; }
    double GetCreepPositionRandom() { return CreepPositionRandom; }


    TArray<TObjectPtr<ABaseCreepActor>>* GetCreeps() { return &Creeps; }
    FVector GetRightCornerCreepLocation();
    FVector GetLeftCornerCreepLocation();

    TArray<FVector> CalculateCreepsPositions(int32 HeightStart, int32 HeightEnd, int32 WidthStart, int32 WidthEnd,
        FVector SquadBaseSpawnLocation, FVector ForwarVectorToNewLocation, bool UseLocationRandom = true);

    FOnSquadIsUnChoisenBySelectionBox OnSquadIsUnChoisenBySelectionBox;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USquadMovementComponent* SquadMovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USquadCalcMovementTargetComponent* SquadCalcTargetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UBoxComponent* SquadSizesBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UInstancedStaticMeshComponent> InstancedNewLocationMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    int32 CreepsNum = 50;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    double CreepPositionRandom = 20.0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    TSubclassOf<ABaseCreepActor> CreepsType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    FCreepsOffsetInSquad CreepsOffsetInSquad;

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
    FVector SquadBaseForwardVector = FVector(1.0, 0.0, 0.0);

    

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad|CreepSpeed")
    FSquadCreepsSpeed CreepsSpeed;


private:
    TArray<TObjectPtr<ABaseCreepActor>> Creeps;
    TArray<FVector> RebuildCreepsNewLocations;
    FVector RebuildSquadNewForwardVector;

    FSquadSizes CurrentSquadSizes;

    TQueue<TObjectPtr<USquadBaseTask>> SquadTasksQueue;
    TObjectPtr<USquadBaseTask> CurrentSquadTask = nullptr;
    bool bCurrentSquadTaskIsExecute             = false;
    bool bSquadIsChosen                         = false;
    bool InstancedMeshNewLocIsSet                = false;

   
    void SetBoxExtendBySquadSize();

    void SpawnCreeps();
    
    FQuat CalculateQuatBeetwenBaseSquadVec(FVector VectorIn);


    void OnRebuildingSquadComplete();

    double CalculateDotFrontSquadToLocation(FVector Location);
    double CalculateDotRightVectorSquadToLocation(FVector Location);
    void ExecuteCurrentTaskQueue();

    FVector CalculateNewSquadCenterOnRebuild();
    FVector CalculateNewRightCorner();
    
    void UpdateInstancedNewLocationMesh(const TArray<FVector>& NewPositions,  const FRotator& NewSquadRotation);
    void DeleteInstancedNewLocationMesh();
};
