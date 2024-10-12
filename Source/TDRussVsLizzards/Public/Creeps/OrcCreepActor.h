// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Creeps/BaseCreepActor.h"
#include "OrcCreepActor.generated.h"

/**
 *
 */
UCLASS()
class TDRUSSVSLIZZARDS_API AOrcCreepActor : public ABaseCreepActor
{
    GENERATED_BODY()

public:
    AOrcCreepActor();

protected:
    virtual void InitSkeletalMesh() override;
    virtual void InitAnimations() override;
    virtual void InitWeapon() override;
};
