// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SquadCalcMovementTargetComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API USquadCalcMovementTargetComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USquadCalcMovementTargetComponent();
    virtual void BeginPlay() override;

protected:
};
