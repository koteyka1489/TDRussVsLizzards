// TD Russ Vs Lizzards Game


#include "Components/CreepMovementComponent.h"

UCreepMovementComponent::UCreepMovementComponent()
{

	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UCreepMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCreepMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

