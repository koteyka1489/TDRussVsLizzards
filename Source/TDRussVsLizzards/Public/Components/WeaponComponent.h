// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TDRUSSVSLIZZARDS_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UWeaponComponent();
	virtual void BeginPlay() override;
	
};
