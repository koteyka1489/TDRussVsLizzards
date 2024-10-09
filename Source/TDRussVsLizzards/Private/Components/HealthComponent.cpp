// TD Russ Vs Lizzards Game

#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHeath;
}

void UHealthComponent::TakeDamage(int32 DamageAmount) 
{
    if (CurrentHealth > 0)
    {
        CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0, MaxHeath);
        if (CurrentHealth <= 0)
        {
            bIsDead = true;
        }
    }
}