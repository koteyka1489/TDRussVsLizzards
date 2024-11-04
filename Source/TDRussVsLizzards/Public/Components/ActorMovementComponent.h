// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorMovementComponent.generated.h"

DECLARE_DELEGATE(FOnMovingComplete)
DECLARE_DELEGATE(FOnRotatingCreepsComplete)
DECLARE_DELEGATE(FOnRotatingFrontSquadComplete)
DECLARE_DELEGATE(FOnRebuildingSquadComplete)

class ABaseCreepActor;
class ABaseSquadCreeps;

enum class ERotateFrontSquadBySide
{
    LeftCorner,
    RightCorner,
    Center
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API UActorMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UActorMovementComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void MoveToLocation(FVector Location);
    void RotateToLocation(FVector Location);
    void RotateFrontSquadToLocation(FVector Location);
    void RotateFrontSquadToLocationFromSide(FVector Location, ERotateFrontSquadBySide SideIn);
    void StopAllMovings();
    void RebuildSquad(const TArray<FVector>& NewCreepLocations);

    FOnMovingComplete OnMovingComplete;
    FOnRotatingCreepsComplete OnRotatingCreepsComplete;
    FOnRotatingFrontSquadComplete OnRotatingFrontSquadComplete;
    FOnRebuildingSquadComplete OnRebuildingSquadComplete;

private:
    TObjectPtr<ABaseSquadCreeps> OwnerSquad;
    TArray<TObjectPtr<ABaseCreepActor>>* CreepsArray;
    TArray<FVector> CreepsLocationFromCenterSquad;
    TArray<FVector> CreepsLocationFromRightCornerSquad;
    TArray<FVector> CreepsLocationFromLeftCornerSquad;
    TArray<FVector> DestinationCreepsToRotateFrontSquadFromCenter;
    TArray<FVector> DestinationCreepsToRebuild;

    FVector DestinationToMoving{};
    FVector DestinationToRotating{};
    FVector DestinationToSquadFrontRotation{};

    bool bDestinationToMovingIsSet             = false;
    bool bDestinationToRotatingIsSet           = false;
    bool bDestinationToSquadFrontRotationIsSet = false;
    bool bRebuildSquadIsSet                    = false;
    bool bAutoOrientToMovement                 = true;
    bool bAutoOrientToFrontSquadRotation       = true;

    float SpeedMoving                    = 600.0f;
    float SpeedRotating                  = 300.0f;
    float InterpSpeed                    = 5.0f;
    int32 CreepEndRotatFrontSquadCounter = 0;
    int32 CreepEndRebuildingCounter = 0;

    void MovingToLocation(float DeltaTime);
    void RotatingToLocationQuat(float DeltaTime);
    void RotatingFrontSquadToLocationFromCenter(float DeltaTime);
    void RebuildingSquad(float DeltaTime);

    void CalculateDestinationCreepsToRotateFrontSquad();
    void CalculateDestinationCreepsToRotateFrontSquadBySide(ERotateFrontSquadBySide Side);
    void UpdateCreepsLocationFromSidesSquad();

};
