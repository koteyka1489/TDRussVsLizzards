// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TDRUSSVSLIZZARDS_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();
    
    void TakeDamage(int32 DamageAmount);
    bool IsDead() { return bIsDead; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "1000"))
    int32 MaxHeath = 100;

private:
    int32 CurrentHealth = 0;
    bool bIsDead        = false;
};
