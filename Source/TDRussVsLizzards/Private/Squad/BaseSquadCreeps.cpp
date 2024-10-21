// TD Russ Vs Lizzards Game

#include "Squad/BaseSquadCreeps.h"
#include "Creeps/BaseCreepActor.h"
#include "Creeps/OrcCreepActor.h"
#include "Creeps/TrollCreepActor.h"
#include "Creeps/BarbarianCreepActor.h"
#include "Components/ActorMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"

ABaseSquadCreeps::ABaseSquadCreeps()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneComponent                = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());

    MovementComponent = CreateDefaultSubobject<UActorMovementComponent>("UActorMovementComponent");
}

void ABaseSquadCreeps::BeginPlay()
{
    Super::BeginPlay();

    Creeps.Reserve(CreepsNum);

    SpawnCreepsN();

    UpdateSquadLocationStart();

    BindOnCreepIsClickedtDelegate();

    MovementComponent->OnMovingComplete.BindUObject(this, &ABaseSquadCreeps::OnMovingComplete);
}

void ABaseSquadCreeps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 5000, FColor::Red, false, -1.f, 0u, 20.0f);
}

void ABaseSquadCreeps::UpdateSquadLocationStart()
{
    FVector SquadRightCorner              = Creeps[0]->GetActorLocation();
    FVector SquadLeftCorner               = Creeps[CurrentSquadSizes.Width - 1]->GetActorLocation();
    FVector SquadRightToLeftCornerHalfVec = (SquadLeftCorner - SquadRightCorner) / 2;

    int32 IndexCreepBackRightCorner = (CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width;
    checkf(Creeps.IsValidIndex(IndexCreepBackRightCorner), TEXT("Invalid Index"));

    FVector BackCorner         = Creeps[IndexCreepBackRightCorner]->GetActorLocation();
    FVector FrontToBackHalfVec = (BackCorner - SquadRightCorner) / 2;

    // Set Center squad on center Spawned creeps
    SetActorLocation(GetActorLocation() + SquadRightToLeftCornerHalfVec + FrontToBackHalfVec);
}

ESquadMovingDirection ABaseSquadCreeps::CalculateSquadMovingDirection(FVector Destination)
{
    FVector VecToDestination = (Destination - GetActorLocation()).GetSafeNormal();
    float Dot                = FVector::DotProduct(VecToDestination, GetActorForwardVector());
    if (Dot >= 0.0)
    {
        return ESquadMovingDirection::FrontMoving;
    }
    else
    {
        return ESquadMovingDirection::BackMoving;
    }
}

void ABaseSquadCreeps::SpawnCreepsN()
{
    CurrentSquadSizes.Heigth = 6;
    CurrentSquadSizes.Width  = CreepsNum / CurrentSquadSizes.Heigth;

    int32 CreepsShortage = CreepsNum - CurrentSquadSizes.Heigth * CurrentSquadSizes.Width;

    const FVector Squadlocation = GetActorLocation();
    FRotator SpawnRotation{0.0, 0.0, 0.0};
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 x = 0; x < CurrentSquadSizes.Heigth; x++)
    {
        for (int32 y = 0; y < CurrentSquadSizes.Width; y++)
        {
            double XLocRand = FMath::FRandRange(-CreepPositionRandom, CreepPositionRandom);
            double YLocRand = FMath::FRandRange(-CreepPositionRandom, CreepPositionRandom);

            const FVector SpawnLocation = FVector(
                Squadlocation.X + XLocRand - 200.0 * (double)x, Squadlocation.Y + YLocRand - 200.0 * (double)y, Squadlocation.Z + 90.0);

            ABaseCreepActor* SpawnedCreep = GetWorld()->SpawnActor<ABaseCreepActor>(CreepsType, SpawnLocation, SpawnRotation, SpawnInfo);
            Creeps.Add(SpawnedCreep);
        }
    }

    for (int32 x = CurrentSquadSizes.Heigth; x < CurrentSquadSizes.Heigth + 1; x++)
    {

        int32 StartSpawnRemainderCreeps = CurrentSquadSizes.Width / 2 - CreepsShortage / 2;
        for (int32 y = StartSpawnRemainderCreeps; y < StartSpawnRemainderCreeps + CreepsShortage; y++)
        {
            double XLocRand = FMath::FRandRange(-CreepPositionRandom, CreepPositionRandom);
            double YLocRand = FMath::FRandRange(-CreepPositionRandom, CreepPositionRandom);

            const FVector SpawnLocation = FVector(
                Squadlocation.X + XLocRand - 200.0 * (double)x, Squadlocation.Y + YLocRand - 200.0 * (double)y, Squadlocation.Z + 90.0);

            ABaseCreepActor* SpawnedCreep = GetWorld()->SpawnActor<ABaseCreepActor>(CreepsType, SpawnLocation, SpawnRotation, SpawnInfo);
            Creeps.Add(SpawnedCreep);
        }
    }
}

FSquadSizes ABaseSquadCreeps::CalculateCurrentSquadSizes()
{
    FSquadSizes Result;

    return Result;
}

void ABaseSquadCreeps::BindOnCreepIsClickedtDelegate()
{

    if (!Creeps.IsEmpty())
    {
        for (auto& Creep : Creeps)
        {
            Creep->OnCreepIsClicked.BindUObject(this, &ABaseSquadCreeps::OnCreepIsClicked);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Creeps array is empty"));
        checkNoEntry();
    }
}

void ABaseSquadCreeps::OnCreepIsClicked()
{
    bSquadIsChosen = true;
    if (OnSquadIsChoisen.ExecuteIfBound(this))
    {}
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnSquadIsChoisen Is not bound"));
        checkNoEntry();
    }

    for (auto& Creep : Creeps)
    {
        Creep->SetCreepIsChoisen(true);
    }
}

void ABaseSquadCreeps::OnMovingComplete()
{
    CurrentAnimation = ESquadCurrentAnimation::Idle;
    for (auto& Creep : Creeps)
    {
        Creep->PlayAnimationIdle();
    }
}

void ABaseSquadCreeps::SquadUnChoisen()
{
    bSquadIsChosen = false;
    for (auto& Creep : Creeps)
    {
        Creep->SetCreepIsChoisen(false);
    }
}

void ABaseSquadCreeps::MoveToLocation(FVector Destination)
{
    MovementComponent->MoveToLocation(Destination);

    if (CurrentAnimation != ESquadCurrentAnimation::Run)
    {
        for (auto& Creep : Creeps)
        {
            Creep->PlayAnimationRun();
        }
        CurrentAnimation = ESquadCurrentAnimation::Run;
    }
}
