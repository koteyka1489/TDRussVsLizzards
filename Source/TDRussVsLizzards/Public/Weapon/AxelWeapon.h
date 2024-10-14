// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "AxelWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TDRUSSVSLIZZARDS_API AAxelWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
    AAxelWeapon();

	protected:
    virtual void SetWeaponMesh() override;
};
