// TD Russ Vs Lizzards Game

#include "Squad/BaseSquadCreeps.h"
#include "Creeps/BaseCreepActor.h"
#include "Creeps/OrcCreepActor.h"
#include "Creeps/TrollCreepActor.h"
#include "Creeps/BarbarianCreepActor.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Team/TeamController.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SquadCalcMovementTargetComponent.h"
#include "Components/SquadMovementComponent.h"
#include "Creeps/CreepArray.h"

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

    SquadMovementComponent   = CreateDefaultSubobject<USquadMovementComponent>("USquadMovementComponent");
    SquadCalcTargetComponent = CreateDefaultSubobject<USquadCalcMovementTargetComponent>("USquadCalcMovementTargetComponent");

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

    Creeps = CreateDefaultSubobject<UCreepArray>("CreepArray");

    
}

void ABaseSquadCreeps::BeginPlay()
{
    Super::BeginPlay();
    SpawnCreeps();
    UpdateSquadLocation();
    

    auto TeamController = Cast<ATeamController>(UGameplayStatics::GetActorOfClass(GetWorld(), ATeamController::StaticClass()));
    if (TeamController)
    {
        SetOwner(TeamController);
    }
}

void ABaseSquadCreeps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000, FColor::Red, false, -1.f, 0u, 20.0f);
    UpdateSquadLocation();
    UpdateBoxExtendBySquadSize();
}

void ABaseSquadCreeps::UpdateSquadLocationStart()
{
    FVector SquadRightCorner              = GetRightCornerCreepLocation();
    FVector SquadLeftCorner               = GetLeftCornerCreepLocation();
    FVector SquadRightToLeftCornerHalfVec = (SquadLeftCorner - SquadRightCorner) / 2;

    int32 IndexCreepBackRightCorner = (CurrentSquadSizes.Heigth - 1) * CurrentSquadSizes.Width;
    

    FVector BackCorner         = GetRightBackCornerCreepLocation();
    FVector FrontToBackHalfVec = (BackCorner - SquadRightCorner) / 2;

    SetActorLocation(GetActorLocation() + SquadRightToLeftCornerHalfVec + FrontToBackHalfVec);
}

void ABaseSquadCreeps::UpdateSquadLocation()
{
    FVector RightFront  = GetRightCornerCreepLocation();
    FVector LeftFront   = GetLeftCornerCreepLocation();
    FVector RightBack   = GetRightBackCornerCreepLocation();
    FVector LeftBack    = GetLeftBackCornerCreepLocation();
    FVector NewLocation = FMath::BiLerp(RightFront, LeftFront, RightBack, LeftBack, 0.5f, 0.5f);
    SetActorLocation(NewLocation);
}

void ABaseSquadCreeps::UpdateSquadRotation(FVector NewSquadForwardVec)
{
    FRotator NewSquadRotation = NewSquadForwardVec.Rotation();
    SetActorRotation(NewSquadRotation);
}

void ABaseSquadCreeps::UpdateSquadNewSizes(int32 NewWidth)
{
    CurrentSquadSizes.Width  = NewWidth;
    CurrentSquadSizes.Heigth = CreepsNum / NewWidth;
}

void ABaseSquadCreeps::UpdateBoxExtendBySquadSize()
{
    FVector RightCorner = GetRightCornerCreepLocation();

    FVector LeftCornerFornt = GetLeftCornerCreepLocation();
    FVector RightBackCorner = GetRightBackCornerCreepLocation();

    double NewWidth  = (LeftCornerFornt - RightCorner).Size() / 2;
    double NewHeight = (RightBackCorner - RightCorner).Size() / 2;

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

    TMap<int32, FVector> SpawnLocations =
        CalculateCreepsPositions(0, CurrentSquadSizes.Heigth, 0, CurrentSquadSizes.Width, Squadlocation, SquadBaseForwardVector);

    int32 StartSpawnRemainderCreeps = CurrentSquadSizes.Width / 2 - CreepsShortage / 2;
    SpawnLocations.Append(CalculateCreepsPositions(CurrentSquadSizes.Heigth, CurrentSquadSizes.Heigth + 1, StartSpawnRemainderCreeps,
        StartSpawnRemainderCreeps + CreepsShortage, Squadlocation, SquadBaseForwardVector));
    
    for (const auto& SpawnLocation : SpawnLocations)
    {
        TObjectPtr<ABaseCreepActor> SpawnedCreep = GetWorld()->SpawnActor<ABaseCreepActor>(CreepsType, SpawnLocation.Value, SpawnRotation, SpawnInfo);
        Creeps->Add(SpawnLocation.Key, SpawnedCreep);
    }
}

TMap<int32, FVector> ABaseSquadCreeps::CalculateCreepsPositions(int32 HeightStart, int32 HeightEnd, int32 WidthStart, int32 WidthEnd,
    FVector SquadBaseSpawnLocation, FVector ForwarVectorToNewLocation, bool UseLocationRandom)
{
    
    TMap<int32, FVector> Result;

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
            int32 Key = UCreepArray::GenerateKey(HeightPos, WidthPos);
            Result.Add(Key, SpawnLocation);
        }
    }

    return Result;
}

FQuat ABaseSquadCreeps::CalculateQuatBeetwenBaseSquadVec(FVector VectorIn)
{
    FQuat Result = FQuat::FindBetweenVectors(SquadBaseForwardVector, VectorIn);

    return Result.GetNormalized();
}

void ABaseSquadCreeps::UpdateRebuildngSquad(int32 NewWidth, FVector NewStartCreepSpawnLocation, FVector NewSquadForwardVerctor)
{
    NewSquadForwardVector = NewSquadForwardVerctor;
    NewSquadWidth         = NewWidth;
    SquadCalcTargetComponent->UpdateNewCreepsPositions(NewWidth, NewStartCreepSpawnLocation, NewSquadForwardVerctor);
    UpdateInstancedNewLocationMesh(
        SquadCalcTargetComponent->GetNewCreepsLocationsNoRandom(), SquadCalcTargetComponent->GetNewSquadRotation());
}

void ABaseSquadCreeps::EndUpdateRebuildingSquad()
{
    UpdateSquadRotation(NewSquadForwardVector);
    UpdateSquadNewSizes(NewSquadWidth);
    DeleteInstancedNewLocationMesh();
    SquadCalcTargetComponent->SetCreepsMovingDestination();
    SquadMovementComponent->SetSquadMovement();
}

void ABaseSquadCreeps::SetSquadIsChoisen()
{
    if (bSquadIsChosen) return;

    bSquadIsChosen = true;

    for (auto& Creep : *Creeps)
    {
        Creep.Value->SetCreepIsChoisen(true);
    }
}

FVector ABaseSquadCreeps::GetRightCornerCreepLocation()
{
    int32 Key = UCreepArray::GenerateKey(0, 0);
    return Creeps->GetValue(Key)->GetActorLocation();
}

FVector ABaseSquadCreeps::GetLeftCornerCreepLocation()
{
    int32 Key = UCreepArray::GenerateKey(0, CurrentSquadSizes.Width - 1);
    return Creeps->GetValue(Key)->GetActorLocation();
}

FVector ABaseSquadCreeps::GetRightBackCornerCreepLocation()
{
    int32 Key = UCreepArray::GenerateKey(CurrentSquadSizes.Heigth - 1, 0);
    return Creeps->GetValue(Key)->GetActorLocation();
}

FVector ABaseSquadCreeps::GetLeftBackCornerCreepLocation()
{
    int32 Key   = UCreepArray::GenerateKey(CurrentSquadSizes.Heigth - 1, CurrentSquadSizes.Width - 1);
    return Creeps->GetValue(Key)->GetActorLocation();
}

void ABaseSquadCreeps::SquadUnChoisen()
{
    if (!bSquadIsChosen) return;

    bSquadIsChosen = false;

    for (auto& Creep : *Creeps)
    {
        Creep.Value->SetCreepIsChoisen(false);
    }
}

void ABaseSquadCreeps::SquadUnChoisenBySelectBox()
{
    if (!bSquadIsChosen) return;

    bSquadIsChosen = false;

    if (OnSquadIsUnChoisenBySelectionBox.ExecuteIfBound(this))
    {
        for (auto& Creep : *Creeps)
        {
            Creep.Value->SetCreepIsChoisen(false);
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
    FVector NewRightCorner = CalculateNewRightCorner(Destination);

    SquadCalcTargetComponent->UpdateNewCreepsPositions(
        CurrentSquadSizes.Width, NewRightCorner, (Destination - GetActorLocation()).GetSafeNormal2D());
    SquadCalcTargetComponent->SetCreepsMovingDestination();
    SquadMovementComponent->SetSquadMovement();
    UpdateSquadRotation((Destination - GetActorLocation()).GetSafeNormal2D());
}

void ABaseSquadCreeps::StopAllMovement()
{
    SquadMovementComponent->StopAllMovings();
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

FVector ABaseSquadCreeps::CalculateNewRightCorner(FVector Destination)
{
    float lenghtCenterToForward = (GetRightCornerCreepLocation() - GetRightBackCornerCreepLocation()).Size() / 2;
    float LenghCenterRight      = (GetRightCornerCreepLocation() - GetLeftCornerCreepLocation()).Size() / 2;

    FQuat QuatBetween        = FQuat::FindBetweenNormals(GetActorForwardVector(), (Destination - GetActorLocation()).GetSafeNormal2D());
    FVector NewRightVector   = QuatBetween.RotateVector(GetActorRightVector());
    FVector NewForwardVector = QuatBetween.RotateVector(GetActorForwardVector());

    FVector NewRightCorner = Destination + NewRightVector * LenghCenterRight + NewForwardVector * lenghtCenterToForward;

    return NewRightCorner;
}

void ABaseSquadCreeps::UpdateInstancedNewLocationMesh(const TMap<int32, FVector>& NewPositions, const FRotator& NewSquadRotation)
{
    if (!InstancedMeshNewLocIsSet)
    {
        InstancedNewLocationMesh->SetVisibility(true);
        InstancedNewLocationMesh->bHiddenInGame = false;

        for (const auto& CreepLocation : NewPositions)
        {
            FTransform NewTransform(NewSquadRotation, CreepLocation.Value, FVector(1.0, 1.0, 1.0));
            InstancedNewLocationMesh->AddInstance(NewTransform);
        }
        InstancedMeshNewLocIsSet = true;
    }
    else
    {
        int32 IndexInstancedMesh = 0;
        for (const auto& CreepLocation : NewPositions)
        {
            FTransform NewTransform(NewSquadRotation, CreepLocation.Value, FVector(1.0, 1.0, 1.0));
            InstancedNewLocationMesh->UpdateInstanceTransform(IndexInstancedMesh, NewTransform, true, true, true);
            IndexInstancedMesh++;
        }
    }
}

void ABaseSquadCreeps::DeleteInstancedNewLocationMesh()
{
    InstancedNewLocationMesh->SetVisibility(false);
    InstancedNewLocationMesh->bHiddenInGame = true;
    InstancedNewLocationMesh->ClearInstances();
    InstancedMeshNewLocIsSet = false;
}
