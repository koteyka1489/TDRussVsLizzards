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
#include "Team/TeamController.h"
#include "Components/InstancedStaticMeshComponent.h"

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

    InstancedNewLocationMesh                    = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancedNewLocationMesh");
    InstancedNewLocationMesh->bDisableCollision = true;
    InstancedNewLocationMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    InstancedNewLocationMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    InstancedNewLocationMesh->SetGenerateOverlapEvents(false);
    check(IsValid(InstancedNewLocationMesh));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SelectCircleMesh(
        TEXT("/Script/Engine.StaticMesh'/Game/Squads/SM_SelectCircle.SM_SelectCircle'"));
    checkf(SelectCircleMesh.Succeeded(), TEXT("Find SelectCircleMesh is not Succeeded "));
    InstancedNewLocationMesh->SetStaticMesh(SelectCircleMesh.Object);
}

void ABaseSquadCreeps::BeginPlay()
{
    Super::BeginPlay();

    Creeps.Reserve(CreepsNum);

    SpawnCreeps();
    UpdateSquadLocationStart();
    SetBoxExtendBySquadSize();

    MovementComponent->OnMovingComplete.BindUObject(this, &ABaseSquadCreeps::OnMovingComplete);
    MovementComponent->OnRotatingCreepsComplete.BindUObject(this, &ABaseSquadCreeps::OnRotatingCreepsComplete);
    MovementComponent->OnRotatingFrontSquadComplete.BindUObject(this, &ABaseSquadCreeps::OnRotatingFrontSquadComplete);
    MovementComponent->OnRebuildingSquadComplete.BindUObject(this, &ABaseSquadCreeps::OnRebuildingSquadComplete);

    auto TeamController = Cast<ATeamController>(UGameplayStatics::GetActorOfClass(GetWorld(), ATeamController::StaticClass()));
    if (TeamController)
    {
        SetOwner(TeamController);
    }

    for (auto& Creep : Creeps)
    {

        float RotatingMove = CreepsSpeed.SpeedRotating + FMath::RandRange(-CreepsSpeed.RotatingRandom, CreepsSpeed.RotatingRandom);
        float SpeedMove    = CreepsSpeed.SpeedMoving + FMath::RandRange(-CreepsSpeed.MovingRandom, CreepsSpeed.MovingRandom);

        Creep->SetCreepSpeeds(RotatingMove, SpeedMove);
    }
}

void ABaseSquadCreeps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000, FColor::Red, false, -1.f, 0u, 20.0f);

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
    FVector RightCornerFrontNewPos = Creeps[0]->GetActorLocation();

    FVector LeftCornerForntNewPos = Creeps[CurrentSquadSizes.Width - 1]->GetActorLocation();
    FVector RightBackCorner       = Creeps[(CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width]->GetActorLocation();

    double NewWidth  = (LeftCornerForntNewPos - RightCornerFrontNewPos).Size() / 2;
    double NewHeight = (RightBackCorner - RightCornerFrontNewPos).Size() / 2;

    FVector NewBoxExtend{NewHeight, NewWidth, 200.0};

    SquadSizesBox->SetBoxExtent(NewBoxExtend);
}

void ABaseSquadCreeps::SpawnCreeps()
{
    CurrentSquadSizes.Heigth = 6;
    CurrentSquadSizes.Width  = CreepsNum / CurrentSquadSizes.Heigth;

    int32 CreepsShortage = CreepsNum - CurrentSquadSizes.Heigth * CurrentSquadSizes.Width;

    const FVector Squadlocation = GetActorLocation();
    FRotator SpawnRotation{0.0, 0.0, 0.0};
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.Owner                          = this;

    TArray<FVector> SpawnLocations =
        CalculateCreepsPositions(0, CurrentSquadSizes.Heigth, 0, CurrentSquadSizes.Width, Squadlocation, SquadBaseForwardVector);

    int32 StartSpawnRemainderCreeps = CurrentSquadSizes.Width / 2 - CreepsShortage / 2;
    SpawnLocations.Append(CalculateCreepsPositions(CurrentSquadSizes.Heigth, CurrentSquadSizes.Heigth + 1, StartSpawnRemainderCreeps,
        StartSpawnRemainderCreeps + CreepsShortage, Squadlocation, SquadBaseForwardVector));

    for (const auto& SpawnLocation : SpawnLocations)
    {
        ABaseCreepActor* SpawnedCreep = GetWorld()->SpawnActor<ABaseCreepActor>(CreepsType, SpawnLocation, SpawnRotation, SpawnInfo);
        Creeps.Add(SpawnedCreep);
    }
}

TArray<FVector> ABaseSquadCreeps::CalculateCreepsPositions(int32 HeightStart, int32 HeightEnd, int32 WidthStart, int32 WidthEnd,
    FVector SquadBaseSpawnLocation, FVector ForwarVectorToNewLocation, bool UseLocationRandom)
{
    TArray<FVector> Result;

    for (int32 HeightPos = HeightStart; HeightPos < HeightEnd; HeightPos++)
    {
        for (int32 WidthPos = WidthStart; WidthPos < WidthEnd; WidthPos++)
        {
            double XLocRand = UseLocationRandom ? FMath::FRandRange(-CreepPositionRandom, CreepPositionRandom) : 0.0;
            double YLocRand = UseLocationRandom ? FMath::FRandRange(-CreepPositionRandom, CreepPositionRandom) : 0.0;

            FVector VecFromBaseSpawnLoc(
                XLocRand - CreepsOffsetInSquad.X * (double)HeightPos, YLocRand - CreepsOffsetInSquad.Y * (double)WidthPos, 0.0);

            FVector VecFromBaseSpawnLocRotated =
                CalculateQuatBeetwenBaseSquadVec(ForwarVectorToNewLocation).RotateVector(VecFromBaseSpawnLoc);

            const FVector SpawnLocation =
                FVector(SquadBaseSpawnLocation.X, SquadBaseSpawnLocation.Y, SquadBaseSpawnLocation.Z + 90.0) + VecFromBaseSpawnLocRotated;

            Result.Add(SpawnLocation);
        }
    }

    return Result;
}

FQuat ABaseSquadCreeps::CalculateQuatBeetwenBaseSquadVec(FVector VectorIn)
{
    FQuat Result = FQuat::FindBetweenVectors(SquadBaseForwardVector, VectorIn);

    return Result.GetNormalized();
}

void ABaseSquadCreeps::RebuildSquad(int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor)
{
    RebuildSquadNewForwardVector = NewSquadForwardVerctor;
    CurrentSquadSizes.Width      = NewWidth;
    CurrentSquadSizes.Heigth     = CreepsNum / CurrentSquadSizes.Width;
    FRotator NewSquadRotation    = NewSquadForwardVerctor.Rotation();

    InstancedNewLocationMesh->SetVisibility(true);
    InstancedNewLocationMesh->bHiddenInGame = false;

    RebuildCreepsNewLocations = CalculateCreepsPositions(
        0, CurrentSquadSizes.Heigth, 0, CurrentSquadSizes.Width, NewStartCreepSpawnLocation, NewSquadForwardVerctor, false);

    int32 CreepsShortage = CreepsNum - CurrentSquadSizes.Heigth * CurrentSquadSizes.Width;
    if (CreepsShortage > 0)
    {
        int32 StartSpawnRemainderCreeps = CurrentSquadSizes.Width / 2 - CreepsShortage / 2;
        RebuildCreepsNewLocations.Append(
            CalculateCreepsPositions(CurrentSquadSizes.Heigth, CurrentSquadSizes.Heigth + 1, StartSpawnRemainderCreeps,
                StartSpawnRemainderCreeps + CreepsShortage, NewStartCreepSpawnLocation, NewSquadForwardVerctor, false));
    }

    RebuildCreepsNewLocations.Add(CalculateNewSquadCenterOnRebuild());

    if (!InstancedMeshNewLocIsSet)
    {
        for (const auto& CreepLocation : RebuildCreepsNewLocations)
        {
            FTransform NewTransform(NewSquadRotation, CreepLocation, FVector(1.0, 1.0, 1.0));
            InstancedNewLocationMesh->AddInstance(NewTransform);
        }

        InstancedMeshNewLocIsSet = true;
    }
    else
    {
        int32 IndexInstancedMesh = 0;
        for (const auto& CreepLocation : RebuildCreepsNewLocations)
        {
            FTransform NewTransform(NewSquadRotation, CreepLocation, FVector(1.0, 1.0, 1.0));
            InstancedNewLocationMesh->UpdateInstanceTransform(IndexInstancedMesh, NewTransform, true, true, true);
            IndexInstancedMesh++;
        }
    }
}

void ABaseSquadCreeps::EndRebuildSquad()
{
    InstancedNewLocationMesh->SetVisibility(false);
    InstancedNewLocationMesh->bHiddenInGame = true;
    InstancedNewLocationMesh->ClearInstances();
    InstancedMeshNewLocIsSet = false;
    MovementComponent->RebuildSquad(RebuildCreepsNewLocations);
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

}

void ABaseSquadCreeps::OnRotatingCreepsComplete()
{
    bCurrentSquadTaskIsExecute = false;
}

void ABaseSquadCreeps::OnRotatingFrontSquadComplete()
{
    bCurrentSquadTaskIsExecute = false;
}

void ABaseSquadCreeps::OnRebuildingSquadComplete()
{
    FVector VectorRebuildRotation = GetActorLocation() + RebuildSquadNewForwardVector * 1000;
    auto RotateCreepsToDestTask   = NewObject<URotateCreepsTask>();
    RotateCreepsToDestTask->InitDestinationTask(VectorRebuildRotation, this);
    SquadTasksQueue.Enqueue(RotateCreepsToDestTask);
    SetBoxExtendBySquadSize();

    FVector VectorRebuildMovement = GetActorLocation() + RebuildSquadNewForwardVector;
    auto MoveToLocationTask       = NewObject<UMoveSquadTask>();
    MoveToLocationTask->InitDestinationTask(VectorRebuildMovement, this);
    SquadTasksQueue.Enqueue(MoveToLocationTask);
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

FVector ABaseSquadCreeps::CalculateNewSquadCenterOnRebuild()
{
    FVector RightCornerFrontNewPos = RebuildCreepsNewLocations[0];
    FVector LeftCornerForntNewPos  = RebuildCreepsNewLocations[CurrentSquadSizes.Width - 1];
    FVector RightBackCorner        = RebuildCreepsNewLocations[(CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width];

    FVector RightToLeftInterp = (LeftCornerForntNewPos - RightCornerFrontNewPos) / 2;
    FVector FrontToBackInterp = (RightBackCorner - RightCornerFrontNewPos) / 2;

    return RebuildCreepsNewLocations[0] + RightToLeftInterp + FrontToBackInterp;
}
