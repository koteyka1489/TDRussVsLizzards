// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Creeps/BaseCreepActor.h"
#include "BarbarianCreepActor.generated.h"

class UWeaponComponent;

UCLASS()
class TDRUSSVSLIZZARDS_API ABarbarianCreepActor : public ABaseCreepActor
{
    GENERATED_BODY()
public:
    ABarbarianCreepActor();

protected:
    UPROPERTY()
    UWeaponComponent* WeaponComponent;

    virtual void InitSkeletalMesh() override;
    virtual void InitAnimations() override;
};
