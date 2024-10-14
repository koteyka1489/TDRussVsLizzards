// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "CudgelWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TDRUSSVSLIZZARDS_API ACudgelWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
    ACudgelWeapon();

protected:
    virtual void SetWeaponMesh() override;
	
};
