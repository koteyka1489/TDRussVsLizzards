// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class ABaseWeapon;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();
    virtual void BeginPlay() override;

    TObjectPtr<ABaseWeapon> GetWeaponInstPtr() { return Weapon; }

    void SetWeaponMesh(TObjectPtr<UStaticMesh> WeaponMeshIn);
    

protected:

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
   TObjectPtr<ABaseWeapon> Weapon;
   

};
