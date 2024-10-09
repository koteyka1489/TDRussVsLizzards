// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDGoal.generated.h"

UCLASS()
class TDRUSSVSLIZZARDS_API ATDGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	ATDGoal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMesh;


};
