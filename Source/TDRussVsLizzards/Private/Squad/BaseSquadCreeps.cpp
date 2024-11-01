// TD Russ Vs Lizzards Game

#include "Squad/BaseSquadCreeps.h"
#include "Creeps/BaseCreepActor.h"
#include "Creeps/OrcCreepActor.h"
#include "Creeps/TrollCreepActor.h"
#include "Creeps/BarbarianCreepActor.h"
#include "Components/ActorMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Squad/Task/MoveSquadTask.h"
#include "Squad/Task/RotateCreepsTask.h"
#include "Squad/Task/RotateFrontSquadTask.h"
#include "Squad/Task/SquadBaseTask.h"
#include "Components/ActorMovementComponent.h"
#include "Camera/CameraPawn.h"
#include "Kismet/GameplayStatics.h"

ABaseSquadCreeps::ABaseSquadCreeps()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    SquadSizesBox = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
    SquadSizesBox->SetBoxExtent(FVector(1.0, 1.0, 1.0));
    SquadSizesBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SquadSizesBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SquadSizesBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    SquadSizesBox->SetVisibility(true);
    SquadSizesBox->bHiddenInGame = false;
    SquadSizesBox->SetupAttachment(GetRootComponent());

    MovementComponent = CreateDefaultSubobject<UActorMovementComponent>("UActorMovementComponent");
}

void ABaseSquadCreeps::BeginPlay()
{
    Super::BeginPlay();

    Creeps.Reserve(CreepsNum);

    SpawnCreepsN();
    UpdateSquadLocationStart();
    SetBoxExtendBySquadSize();

    MovementComponent->OnMovingComplete.BindUObject(this, &ABaseSquadCreeps::OnMovingComplete);
    MovementComponent->OnRotatingCreepsComplete.BindUObject(this, &ABaseSquadCreeps::OnRotatingCreepsComplete);
    MovementComponent->OnRotatingFrontSquadComplete.BindUObject(this, &ABaseSquadCreeps::OnRotatingFrontSquadComplete);

    auto CameraPawn = Cast<ACameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraPawn::StaticClass()));
    if (CameraPawn)
    {
        SetOwner(CameraPawn);
    }
}

void ABaseSquadCreeps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 5000, FColor::Red, false, -1.f, 0u, 20.0f);

    ExecuteCurrentTaskQueue();
}

void ABaseSquadCreeps::UpdateSquadLocationStart()
{
    FVector SquadRightCorner              = GetRightCornerCreepLocation();
    FVector SquadLeftCorner               = GetLeftCornerCreepLocation();
    FVector SquadRightToLeftCornerHalfVec = (SquadLeftCorner - SquadRightCorner) / 2;

    int32 IndexCreepBackRightCorner = (CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width;
    checkf(Creeps.IsValidIndex(IndexCreepBackRightCorner), TEXT("Invalid Index"));

    FVector BackCorner         = Creeps[IndexCreepBackRightCorner]->GetActorLocation();
    FVector FrontToBackHalfVec = (BackCorner - SquadRightCorner) / 2;

    // Set Center squad on center Spawned creeps
    SetActorLocation(GetActorLocation() + SquadRightToLeftCornerHalfVec + FrontToBackHalfVec);
}

void ABaseSquadCreeps::SetBoxExtendBySquadSize()
{
    FVector SquadRightCorner = GetRightCornerCreepLocation();

    FVector NewBoxExtend = GetActorLocation() - SquadRightCorner;
    NewBoxExtend         = NewBoxExtend.GetAbs();
    NewBoxExtend.Z       = 200.0;

    SquadSizesBox->SetBoxExtent(NewBoxExtend);
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
    SpawnInfo.Owner                          = this;

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

void ABaseSquadCreeps::SetSquadIsChoisen()
{
    if (bSquadIsChosen) return;

    bSquadIsChosen = true;

    for (auto& Creep : Creeps)
    {
        Creep->SetCreepIsChoisen(true);
    }
}

FVector ABaseSquadCreeps::GetRightCornerCreepLocation()
{
    return Creeps[0]->GetActorLocation();
}

FVector ABaseSquadCreeps::GetLeftCornerCreepLocation()
{
    return Creeps[CurrentSquadSizes.Width - 1]->GetActorLocation();
}

void ABaseSquadCreeps::OnMovingComplete()
{
    bCurrentSquadTaskIsExecute = false;
    CurrentAnimation           = ESquadCurrentAnimation::Idle;
    for (auto& Creep : Creeps)
    {
        Creep->PlayAnimationIdle();
    }
}

void ABaseSquadCreeps::OnRotatingCreepsComplete()
{
    bCurrentSquadTaskIsExecute = false;
}

void ABaseSquadCreeps::OnRotatingFrontSquadComplete()
{
    bCurrentSquadTaskIsExecute = false;
}

void ABaseSquadCreeps::SquadUnChoisen()
{
    if (!bSquadIsChosen) return;

    bSquadIsChosen = false;

    for (auto& Creep : Creeps)
    {
        Creep->SetCreepIsChoisen(false);
    }
}

void ABaseSquadCreeps::SquadUnChoisenBySelectBox()
{
    if (!bSquadIsChosen) return;

    bSquadIsChosen = false;

    if (OnSquadIsUnChoisenBySelectionBox.ExecuteIfBound(this))
    {
        for (auto& Creep : Creeps)
        {
            Creep->SetCreepIsChoisen(false);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("OnSquadIsUnChoisenBySelectionBox Is not bound"));
        checkNoEntry();
    }
}

void ABaseSquadCreeps::MoveAndRotatingSquadToLocation(FVector Destination)
{
    StopAllTasks();

    double DotSquadToDestination = CalculateDotFrontSquadToLocation(Destination);
    if (DotSquadToDestination >= 0.0)
    {
        auto RotateFrontTask = NewObject<URotateFrontSquadTask>();
        RotateFrontTask->InitDestinationTask(Destination, this);
        RotateFrontTask->InitSideRotating(ERotateFrontSquadBySide::Center);
        SquadTasksQueue.Enqueue(RotateFrontTask);

        auto RotateCreepsToDestTask = NewObject<URotateCreepsTask>();
        RotateCreepsToDestTask->InitDestinationTask(Destination, this);
        SquadTasksQueue.Enqueue(RotateCreepsToDestTask);

        auto MoveToLocationTask = NewObject<UMoveSquadTask>();
        MoveToLocationTask->InitDestinationTask(Destination, this);
        SquadTasksQueue.Enqueue(MoveToLocationTask);
    }
    else
    {
        FVector TurnAroundVector = GetActorLocation() + (-GetActorForwardVector() * 1000);
        auto TurnAroundTask      = NewObject<URotateCreepsTask>();
        TurnAroundTask->InitDestinationTask(TurnAroundVector, this);
        SquadTasksQueue.Enqueue(TurnAroundTask);

        auto RotateFrontTask = NewObject<URotateFrontSquadTask>();
        RotateFrontTask->InitDestinationTask(Destination, this);
        RotateFrontTask->InitSideRotating(ERotateFrontSquadBySide::Center);
        SquadTasksQueue.Enqueue(RotateFrontTask);

        auto RotateCreepsToDestTask = NewObject<URotateCreepsTask>();
        RotateCreepsToDestTask->InitDestinationTask(Destination, this);
        SquadTasksQueue.Enqueue(RotateCreepsToDestTask);

        auto MoveToLocationTask = NewObject<UMoveSquadTask>();
        MoveToLocationTask->InitDestinationTask(Destination, this);
        SquadTasksQueue.Enqueue(MoveToLocationTask);
    }
}

void ABaseSquadCreeps::PlayRunAnimation()
{
    if (CurrentAnimation != ESquadCurrentAnimation::Run)
    {
        for (auto& Creep : Creeps)
        {
            Creep->PlayAnimationRun();
        }
        CurrentAnimation = ESquadCurrentAnimation::Run;
    }
}

void ABaseSquadCreeps::StopAllTasks()
{
    MovementComponent->StopAllMovings();
    CurrentSquadTask           = nullptr;
    bCurrentSquadTaskIsExecute = false;
    SquadTasksQueue.Empty();
}

double ABaseSquadCreeps::CalculateDotFrontSquadToLocation(FVector Location)
{
    FVector SquadForwardVector             = GetActorForwardVector();
    FVector SquadToLocationNormalizeVector = (Location - GetActorLocation()).GetSafeNormal2D();
    return SquadForwardVector.Dot(SquadToLocationNormalizeVector);
}

double ABaseSquadCreeps::CalculateDotRightVectorSquadToLocation(FVector Location)
{
    FVector SquadRightVector               = GetActorRightVector();
    FVector SquadToLocationNormalizeVector = (Location - GetActorLocation()).GetSafeNormal2D();
    return SquadRightVector.Dot(SquadToLocationNormalizeVector);
}

ERotateFrontSquadBySide ABaseSquadCreeps::GetSideToFrontSquadRotating(FVector Location)
{
    ERotateFrontSquadBySide Result;
    double DotRightVectorToLocation = CalculateDotRightVectorSquadToLocation(Location);
    if (DotRightVectorToLocation >= 0.0)
    {
        Result = ERotateFrontSquadBySide::RightCorner;
    }
    else
    {
        Result = ERotateFrontSquadBySide::LeftCorner;
    }

    return Result;
}

void ABaseSquadCreeps::ExecuteCurrentTaskQueue()
{

    TObjectPtr<USquadBaseTask> FirstQueueTask;
    if (!SquadTasksQueue.Peek(FirstQueueTask) || bCurrentSquadTaskIsExecute || SquadTasksQueue.IsEmpty()) return;

    if (CurrentSquadTask == FirstQueueTask)
    {
        return;
    }
    else
    {
        TObjectPtr<USquadBaseTask> NewCurrenWueueTask;
        if (SquadTasksQueue.Dequeue(NewCurrenWueueTask))
        {
            bCurrentSquadTaskIsExecute = true;
            CurrentSquadTask           = NewCurrenWueueTask;
            CurrentSquadTask->ExecuteTask();
        }
    }
}
