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
    FVector GetRightCornerCreepLocation() const;
    
    void MoveAndRotatingSquadToLocation(FVector Destination);
    void StopAllMovement();

    void UpdateRebuildngSquad(int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor);
    void EndUpdateRebuildingSquad();

    int32 GetCreepsNum() const { return CreepsNum; }
    FCreepsOffsetInSquad GetCreepsOffsetInSquad() const { return CreepsOffsetInSquad; }
    double GetCreepPositionRandom() const { return CreepPositionRandom; }
    TObjectPtr<UCreepArray> GetCreeps() { return Creeps; }
    FSquadSizes GetCurrentSquadSizes() const {return CurrentSquadSizes; }

    TMap<int32, FVector> CalculateCreepsPositions(int32 HeightStart, int32 HeightEnd, int32 WidthStart, int32 WidthEnd,
        FVector SquadBaseSpawnLocation, FVector ForwarVectorToNewLocation, bool UseLocationRandom = true);

    FOnSquadIsUnChoisenBySelectionBox OnSquadIsUnChoisenBySelectionBox;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USquadMovementComponent* SquadMovementComponent;


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
    FRotator NewSquadRotation;
    int32 NewSquadWidth;
    TMap<int32, FVector> NewCreepsLocations;
    TMap<int32, FVector> NewCreepsLocationsNoRandom;
    
    bool bSquadIsChosen                         = false;
    bool InstancedMeshNewLocIsSet               = false;
    
    FVector GetLeftCornerCreepLocation() const;
    FVector GetRightBackCornerCreepLocation() const;
    FVector GetLeftBackCornerCreepLocation() const;

    void UpdateBoxExtendBySquadSize();
    void UpdateSquadLocationStart();
    void UpdateSquadLocation();
    void UpdateSquadRotation(const FVector& NewSquadForwarVec);
    void UpdateSquadNewSizes();
    void UpdateNewCreepsPositions(int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor);
    void UpdateSquadPositionKeys();
    void UpdateSquadPositionKeysSmart();
    
    void SetCreepsMovingDestination();

    void SpawnCreeps();
    FQuat CalculateQuatBeetwenBaseSquadVec(FVector VectorIn);
    double CalculateDotFrontSquadToLocation(FVector Location);
    double CalculateDotRightVectorSquadToLocation(FVector Location);
    FVector CalculateNewRightCorner(FVector Destination);
    void UpdateInstancedNewLocationMesh();
    void DeleteInstancedNewLocationMesh();
    bool CalculateSquadIsTurnAround(FVector NewSquadDirection);
};
