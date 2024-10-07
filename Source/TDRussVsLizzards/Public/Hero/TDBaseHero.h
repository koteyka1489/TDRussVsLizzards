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
	ATDBaseHero();
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;


	private:
    void OnSetHeroDestination(FVector HeroDestination);

};
