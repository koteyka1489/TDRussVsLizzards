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

    void InitWeaponType(TSubclassOf<ABaseWeapon> WeaponTypeIn);

protected:
    UPROPERTY()
    TSubclassOf<ABaseWeapon> WeaponType;

    UPROPERTY()
    TObjectPtr<ABaseWeapon> WeaponInst;

private:
    bool AttachWeaponToSocket();
};
