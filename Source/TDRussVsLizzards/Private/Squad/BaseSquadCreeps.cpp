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

    // SpawnCreeps();
    SpawnCreepsN();

    BindOnCreepIsClickedtDelegate();

    MovementComponent->OnMovingComplete.BindUObject(this, &ABaseSquadCreeps::OnMovingComplete);
}

void ABaseSquadCreeps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000, FColor::Red, false, -1.f, 0u, 5.0f);
}

void ABaseSquadCreeps::SpawnCreeps()
{
    int32 CreepsRows            = FMath::FloorToInt(FMath::Sqrt((float)CreepsNum));
    int32 CreepsColumns         = CreepsRows;
    int32 CreepsRemainder       = CreepsNum - CreepsRows * CreepsColumns;
    const FVector Squadlocation = GetActorLocation();
    FRotator SpawnRotation{0.0, 0.0, 0.0};

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    for (int32 y = 0; y < CreepsRows; y++)
    {
        for (int32 x = 0; x < CreepsColumns; x++)
        {

            const FVector SpawnLocation =
                FVector(Squadlocation.X + 200.0 * (double)x, Squadlocation.Y + 200.0 * (double)y, Squadlocation.Z + 90.0);

            ABaseCreepActor* SpawnedCreep = GetWorld()->SpawnActor<ABaseCreepActor>(CreepsType, SpawnLocation, SpawnRotation, SpawnInfo);
            Creeps.Add(SpawnedCreep);
        }
    }

    for (int32 y = CreepsRows; y < CreepsRows + 1; y++)
    {
        for (int32 x = 0; x < CreepsRemainder; x++)
        {
            const FVector SpawnLocation =
                FVector(Squadlocation.X + 200.0 * (double)x, Squadlocation.Y + 200.0 * (double)y, Squadlocation.Z + 90.0);

            ABaseCreepActor* SpawnedCreep = GetWorld()->SpawnActor<ABaseCreepActor>(CreepsType, SpawnLocation, SpawnRotation, SpawnInfo);
            Creeps.Add(SpawnedCreep);
        }
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
    MovementComponent->MoveToLocation(Destination);
    for (auto& Creep : Creeps)
    {
        Creep->PlayAnimationRun();
    }
}
