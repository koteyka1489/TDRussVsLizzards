// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TDPawnMovementComponent.generated.h"

UCLASS()
class TDRUSSVSLIZZARDS_API UTDPawnMovementComponent : public UPawnMovementComponent
{
    GENERATED_BODY()

public:
    UTDPawnMovementComponent();
    void MoveToLocation(FVector Location);
    void RotateToLocation(FVector Location);

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SpeedMoving = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SpeedRotating = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    bool bAutoOrientToMovement = false;

private:
    FVector DestinationToMoving{};
    FVector DestinationToRotating{};
    bool bDestinationToMovingIsSet   = false;
    bool bDestinationToRotatingIsSet = false;

    void MovingToLocation(float DeltaTime);
    void RotatingToLocation(float DeltaTime);

};