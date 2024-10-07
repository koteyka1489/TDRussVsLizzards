// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceMarinePawn.generated.h"

UCLASS()
class TDRUSSVSLIZZARDS_API ASpaceMarinePawn : public APawn
{
    GENERATED_BODY()

public:
    ASpaceMarinePawn();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* SkeletalMeshComponent;

protected:
    virtual void BeginPlay() override;
};
