// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeamController.generated.h"

class ABaseSquadCreeps;
class ATDCameraController;
class ASelectionBox;

USTRUCT(BlueprintType)
struct FRebuidSquadClampWidth
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelectionBox")
    int32 Min = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelectionBox")
    int32 Max = 60;
};

UCLASS()
class TDRUSSVSLIZZARDS_API ATeamController : public AActor
{
    GENERATED_BODY()

public:
    ATeamController();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void SetSquadIsChoisen(TObjectPtr<ABaseSquadCreeps> ChoisenSquad);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelectionBox")
    TSubclassOf<ASelectionBox> SelectionBoxDefaultClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelectionBox")
    FRebuidSquadClampWidth RebuidSquadClampWidth;

private:
    TObjectPtr<ATDCameraController> CameraController;
    TArray<TObjectPtr<ABaseSquadCreeps>> SquadsOnLevel;
    TArray<TObjectPtr<ABaseSquadCreeps>> ChoisenSquads;
    bool bMultiplySelectSquadByClick       = false;
    bool bMultiplySelectSquadBySelectedBox = false;

    TObjectPtr<ASelectionBox> SelectionBox;
    FVector SelectionBoxStartLocation = FVector::Zero();
    bool bBoxIsSpawned                = false;

    FVector RebuildSquadStartLocation = FVector::Zero();
    bool bRebuildSquadIsContinue      = false;
    static constexpr float WidthOffsetsOnMultSquadRebuild = 200.0f;

    void OnLeftMouseClickChois(FHitResult Hit);
    void OnLeftMouseHold();
    void OnLeftMouseHoldCompleted();

    void OnRightMouseClick(FHitResult Hit);
    void OnRightMouseHold();
    void OnRightMouseHoldCompleted();

    void OnSquadIsUnChoisen(ABaseSquadCreeps* SquadIn);
    void OnMultiplySelectSquad(bool Value);

    void OnStopSquad();
    void AddSquadToChoisenSquadsArray(ABaseSquadCreeps* SquadIn);

    void GetSquadsOnLevel();
    void BindOnSquadIsChoisenDelegate();
    void UnchoiseCurrentSquad();

    void CreateSelectionBox();

    void UpdateRebuildSquad();
    int32 CalculateNewWidthSquad(double LengthRebuildVector);
    TArray<int32> CalculateMultiplySquadWidth(double LengthRebuildVector) const;
    FVector CalculateRebuildForwardVector( FVector EndPoint) const;
    TArray<TObjectPtr<ABaseSquadCreeps>> CalculateSquadsFromEndPoint(FVector EndPoint) const;
};



