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

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000, FColor::Red, false, -1.f, 0u, 5.0f);
}

void ABaseSquadCreeps::UpdateSquadLocationStart()
{
    FVector SquadRightCorner          = Creeps[0]->GetActorLocation();
    FVector SquadLeftCorner           = Creeps[CurrentSquadSizes.Width - 1]->GetActorLocation();
    FVector SquadRightToLeftCornerVec = SquadLeftCorner - SquadRightCorner;
    SetActorLocation(GetActorLocation() + SquadRightToLeftCornerVec / 2);
}

void ABaseSquadCreeps::UpdateSquadLocationWhenChangeDirection(ESquadMovingDirection Direction)
{
    if (Direction == ESquadMovingDirection::FrontMoving || Direction == ESquadMovingDirection::BackMoving)
    {
        if (Creeps.Num() == 0) return;

        FVector FrontCorner             = Creeps[0]->GetActorLocation();
        int32 IndexCreepBackRightCorner = (CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width;

        if (!Creeps.IsValidIndex(IndexCreepBackRightCorner)) return;

        FVector BackCorner     = Creeps[IndexCreepBackRightCorner]->GetActorLocation();
        FVector FrontToBackVec = BackCorner - FrontCorner;
        SetActorLocation(GetActorLocation() + FrontToBackVec * FrontBackMovingMultiplier);
        if (FrontBackMovingMultiplier == 1)
        {
            FrontBackMovingMultiplier = -1;
        }
        else
        {
            FrontBackMovingMultiplier = 1;
        }
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
    ESquadMovingDirection CalcMovingDirection = CalculateSquadMovingDirection(Destination);
    if (CalcMovingDirection != CurrentMovingDirection)
    {
        UpdateSquadLocationWhenChangeDirection(CalcMovingDirection);
    }

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
