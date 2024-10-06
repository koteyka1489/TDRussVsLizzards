// TD Russ Vs Lizzards Game


#include "Hero/TDBaseHero.h"

// Sets default values
ATDBaseHero::ATDBaseHero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATDBaseHero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATDBaseHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATDBaseHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

