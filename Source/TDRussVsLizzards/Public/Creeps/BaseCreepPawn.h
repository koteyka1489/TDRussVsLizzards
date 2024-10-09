// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseCreepPawn.generated.h"

UCLASS()
class TDRUSSVSLIZZARDS_API ABaseCreepPawn : public APawn
{
    GENERATED_BODY()

public:
    ABaseCreepPawn();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;

private:
};
