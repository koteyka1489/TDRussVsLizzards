// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSquadCreeps.generated.h"

DECLARE_DELEGATE_OneParam(FOnSquadIsUnChoisenBySelectionBox, ABaseSquadCreeps*);

class ABaseCreepActor;
class UBoxComponent;
class USceneComponent;
class UInstancedStaticMeshComponent;
class USquadMovementComponent;
class USquadCalcMovementTargetComponent;
class UCreepArray;

struct FSquadSizes
{
    int32 Width;
    int32 Heigth;
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

    void SetSquadIsChoisen();
    void SquadUnChoisen();
    void SquadUnChoisenBySelectBox();
   
    void MoveAndRotatingSquadToLocation(FVector Destination);
    void StopAllMovement();

    void UpdateRebuildngSquad(int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor);
    void EndUpdateRebuildingSquad();

    int32 GetCreepsNum() { return CreepsNum; }
    FCreepsOffsetInSquad GetCreepsOffsetInSquad() { return CreepsOffsetInSquad; }
    double GetCreepPositionRandom() { return CreepPositionRandom; }
    TObjectPtr<UCreepArray> GetCreeps() { return Creeps; }

    TMap<int32, FVector> CalculateCreepsPositions(int32 HeightStart, int32 HeightEnd, int32 WidthStart, int32 WidthEnd,
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

private:
    UPROPERTY()
    TObjectPtr<UCreepArray> Creeps;

    FSquadSizes CurrentSquadSizes;
    FVector NewSquadForwardVector = FVector::Zero();
    int32 NewSquadWidth;

    bool bSquadIsChosen                         = false;
    bool InstancedMeshNewLocIsSet               = false;

    FVector GetRightCornerCreepLocation();
    FVector GetLeftCornerCreepLocation();
    FVector GetRightBackCornerCreepLocation();
    FVector GetLeftBackCornerCreepLocation();

    void UpdateBoxExtendBySquadSize();
    void UpdateSquadLocationStart();
    void UpdateSquadLocation();
    void UpdateSquadRotation(FVector NewSquadForwardVector);
    void UpdateSquadNewSizes(int32 NewWidth);
    void UpdateSquadPostionKeys(const TMap<int32, FVector>& NewPositions);

    void SpawnCreeps();
    FQuat CalculateQuatBeetwenBaseSquadVec(FVector VectorIn);
    double CalculateDotFrontSquadToLocation(FVector Location);
    double CalculateDotRightVectorSquadToLocation(FVector Location);
    FVector CalculateNewRightCorner(FVector Destination);
    void UpdateInstancedNewLocationMesh(const TMap<int32, FVector>& NewPositions, const FRotator& NewSquadRotation);
    void DeleteInstancedNewLocationMesh();
};
