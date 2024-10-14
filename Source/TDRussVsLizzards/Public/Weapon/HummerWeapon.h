// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "HummerWeapon.generated.h"

/**
 *
 */
UCLASS()
class TDRUSSVSLIZZARDS_API AHummerWeapon : public ABaseWeapon
{
    GENERATED_BODY()

public:
    AHummerWeapon();

protected:
    virtual void SetWeaponMesh() override;
};
