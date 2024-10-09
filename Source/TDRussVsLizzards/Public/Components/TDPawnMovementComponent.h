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

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Speed = 400.0f;

private:
    FVector Destination{};
    bool bDestinationIsSet = false;


    void MoveToLocation(float DeltaTime);
};
