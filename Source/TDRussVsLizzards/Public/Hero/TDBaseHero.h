// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TDBaseHero.generated.h"

UCLASS()
class TDRUSSVSLIZZARDS_API ATDBaseHero : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATDBaseHero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
