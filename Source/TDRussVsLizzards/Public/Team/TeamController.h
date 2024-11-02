// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeamController.generated.h"

class ABaseSquadCreeps;
class ATDCameraController;
class ASelectionBox;

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

private:
    TObjectPtr<ATDCameraController> CameraController;
    TArray<TObjectPtr<ABaseSquadCreeps>> SquadsOnLevel;
    TArray<TObjectPtr<ABaseSquadCreeps>> ChoisenSquads;
    bool bMultiplySelectSquadByClick       = false;
    bool bMultiplySelectSquadBySelectedBox = false;

    TObjectPtr<ASelectionBox> SelectionBox;
    FVector SelectionBoxStartLocation;
    bool bBoxIsSpawned = false;

    void OnLeftMouseClickChois(FHitResult Hit);
    void OnRightMouseClick(FHitResult Hit);
    void OnSquadIsUnChoisen(ABaseSquadCreeps* SquadIn);
    void OnMultiplySelectSquad(bool Value);
    void OnLeftMouseHold();
    void OnLeftMouseHoldCompleted();
    void OnStopSquad();
    void AddSquadToChoisenSquadsArray(ABaseSquadCreeps* SquadIn);

    void GetSquadsOnLevel();
    void BindOnSquadIsChoisenDelegate();
    void UnchoiseCurrentSquad();
    void CreateSelectionBox();
};