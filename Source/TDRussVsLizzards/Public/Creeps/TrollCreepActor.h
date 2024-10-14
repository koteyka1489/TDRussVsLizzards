// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Creeps/BaseCreepActor.h"
#include "TrollCreepActor.generated.h"

/**
 *
 */
UCLASS()
class TDRUSSVSLIZZARDS_API ATrollCreepActor : public ABaseCreepActor
{
    GENERATED_BODY()

public:
    ATrollCreepActor();

protected:
    UPROPERTY()
    UWeaponComponent* WeaponComponent;

    virtual void InitSkeletalMesh() override;
    virtual void InitAnimations() override;
};
