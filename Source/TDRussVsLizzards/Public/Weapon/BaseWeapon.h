// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class UStaticMeshComponent;

UCLASS()
class TDRUSSVSLIZZARDS_API ABaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ABaseWeapon();
    virtual void BeginPlay() override;
    

protected:
    UPROPERTY()
    UStaticMeshComponent* WeaponStaticMesh;

    virtual void SetWeaponMesh();
};
