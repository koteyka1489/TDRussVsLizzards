// TD Russ Vs Lizzards Game

#include "Squad/BaseSquadCreeps.h"
#include "Creeps/BaseCreepActor.h"
#include "Creeps/OrcCreepActor.h"
#include "Creeps/TrollCreepActor.h"
#include "Creeps/BarbarianCreepActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/ActorMovementComponent.h"
#include "DrawDebugHelpers.h"

ABaseSquadCreeps::ABaseSquadCreeps()
{
    PrimaryActorTick.bCanEverTick = true;

    MovementComponent = CreateDefaultSubobject<UActorMovementComponent>("UActorMovementComponent");
}

void ABaseSquadCreeps::BeginPlay()
{
    Super::BeginPlay();

    Creeps.Reserve(CreepsNum);

    SpawnCreepsN();

    UpdateSquadLocation(CurrentSquadMovingDirection);

    BindOnCreepIsClickedtDelegate();

    MovementComponent->OnMovingComplete.BindUObject(this, &ABaseSquadCreeps::OnMovingComplete);
}

void ABaseSquadCreeps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000, FColor::Red, false, -1.f, 0u, 5.0f);
}

void ABaseSquadCreeps::UpdateSquadLocation(ESquadMovingDirection Direction)
{
    switch (Direction)
    {
        case ESquadMovingDirection::FrontMoving:
        {
            FVector SquadRightCorner          = Creeps[0]->GetActorLocation();
            FVector SquadLeftCorner           = Creeps[CurrentSquadSizes.Width - 1]->GetActorLocation();
            FVector SquadRightToLeftCornerVec = SquadLeftCorner - SquadRightCorner;
            SetActorLocation(GetActorLocation() + SquadRightToLeftCornerVec / 2);
            break;
        }
        case ESquadMovingDirection::BackMoving:
        {
            int32 IndexCreepBackRightCorner       = (CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width;
            FVector SquadBackRightCorner          = Creeps[IndexCreepBackRightCorner]->GetActorLocation();
            int32 IndexCreepBackLeftCorner        = CurrentSquadSizes.Heigth * CurrentSquadSizes.Width;
            FVector SquadBackLeftCorner           = Creeps[IndexCreepBackLeftCorner]->GetActorLocation();
            FVector SquadBackRightToLeftCornerVec = SquadBackLeftCorner - SquadBackRightCorner;

            FVector SquadFrontToBackVec = SquadBackRightCorner - Creeps[0]->GetActorLocation();

            SetActorLocation(GetActorLocation() + SquadFrontToBackVec + SquadBackRightToLeftCornerVec / 2);
            break;
        }
        case ESquadMovingDirection::LeftMoving: break;
        case ESquadMovingDirection::RightMoving: break;
        default: break;
    }
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

            const FVector SpawnLocation =
                FVector(Squadlocation.X - 200.0 * (double)x, Squadlocation.Y - 200.0 * (double)y, Squadlocation.Z + 90.0);

            ABaseCreepActor* SpawnedCreep = GetWorld()->SpawnActor<ABaseCreepActor>(CreepsType, SpawnLocation, SpawnRotation, SpawnInfo);
            Creeps.Add(SpawnedCreep);
        }
    }

    for (int32 x = CurrentSquadSizes.Heigth; x < CurrentSquadSizes.Heigth + 1; x++)
    {
        int32 StartSpawnRemainderCreeps = CurrentSquadSizes.Width / 2 - CreepsShortage / 2;
        for (int32 y = StartSpawnRemainderCreeps; y < StartSpawnRemainderCreeps + CreepsShortage; y++)
        {

            const FVector SpawnLocation =
                FVector(Squadlocation.X - 200.0 * (double)x, Squadlocation.Y - 200.0 * (double)y, Squadlocation.Z + 90.0);

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
    checkf(!Creeps.IsEmpty(), TEXT("Creeps array is empty"));

    for (auto& Creep : Creeps)
    {
        Creep->OnCreepIsClicked.BindUObject(this, &ABaseSquadCreeps::OnCreepIsClicked);
    }
}

void ABaseSquadCreeps::OnCreepIsClicked()
{
    bSquadIsChosen = true;

    checkf(OnSquadIsChoisen.ExecuteIfBound(this), TEXT("OnSquadIsChoisen Is not bound"));

    for (auto& Creep : Creeps)
    {
        Creep->SetCreepIsChoisen(true);
    }
}

void ABaseSquadCreeps::OnMovingComplete()
{
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
    ESquadMovingDirection CalcMovingDirection = CalculateSquadMovingDirection(Destination);
    if (CalcMovingDirection != CurrentSquadMovingDirection)
    {
        UpdateSquadLocation(CalcMovingDirection);
    }

    MovementComponent->MoveToLocation(Destination);
    for (auto& Creep : Creeps)
    {
        Creep->PlayAnimationRun();
    }
}
