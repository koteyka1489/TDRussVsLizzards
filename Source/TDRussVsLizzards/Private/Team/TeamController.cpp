// TD Russ Vs Lizzards Game

#include "Team/TeamController.h"
#include "Squad/BaseSquadCreeps.h"
#include "Camera/TDCameraController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Camera/SelectionBox.h"
#include "DrawDebugHelpers.h"
#include "Squad/SquadsGroup.h"

ATeamController::ATeamController()
{
    PrimaryActorTick.bCanEverTick = true;

    
}

void ATeamController::BeginPlay()
{
    Super::BeginPlay();

    CameraController = Cast<ATDCameraController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (CameraController)
    {
        CameraController->OnLeftMouseClickChois.BindUObject(this, &ATeamController::OnLeftMouseClickChois);
        CameraController->OnRightMouseClickChois.BindUObject(this, &ATeamController::OnRightMouseClick);
        CameraController->OnMultiplySelectSquad.BindUObject(this, &ATeamController::OnMultiplySelectSquad);
        CameraController->OnLeftMouseHold.BindUObject(this, &ATeamController::OnLeftMouseHold);
        CameraController->OnLeftMouseHoldCompleted.BindUObject(this, &ATeamController::OnLeftMouseHoldCompleted);
        CameraController->OnStopSquad.BindUObject(this, &ATeamController::OnStopSquad);
        CameraController->OnRightMouseHold.BindUObject(this, &ATeamController::OnRightMouseHold);
        CameraController->OnRightMouseHoldCompleted.BindUObject(this, &ATeamController::OnRightMouseHoldCompleted);
        CameraController->OnGroupingSquad.BindUObject(this, &ATeamController::OnGroupingSquad);
    }

    GetSquadsOnLevel();
    BindOnSquadIsChoisenDelegate();
}

void ATeamController::Tick(float DeltaTime)
{
    if (bBoxIsSpawned)
    {
        SelectionBox->Update(CameraController->GetMouseLocationOnTerrain());
    }
    if (bRebuildSquadIsContinue)
    {
        UpdateRebuildSquad();
    }
}

void ATeamController::SetSquadIsChoisen(TObjectPtr<ABaseSquadCreeps> ChoisenSquad)
{
    ChoisenSquad->SetSquadIsChoisen();
    AddSquadToChoisenSquadsArray(ChoisenSquad);
}

void ATeamController::OnLeftMouseClickChois(FHitResult Hit)
{
    auto Box = Cast<UBoxComponent>(Hit.GetComponent());
    if (!Box)
    {
        UnchoiseCurrentSquad();
        return;
    }

    auto Squad = Cast<ABaseSquadCreeps>(Box->GetOwner());

    if (!Squad)
    {
        UnchoiseCurrentSquad();
        return;
    }

    SetSquadIsChoisen(Squad);
}

void ATeamController::OnLeftMouseHold()
{
    if (!bBoxIsSpawned)
    {
        CreateSelectionBox();
        bBoxIsSpawned = true;
        bMultiplySelectSquadBySelectedBox = true;
    }
}

void ATeamController::OnLeftMouseHoldCompleted()
{
    if (bBoxIsSpawned)
    {
        SelectionBox->SelectionComplete();
        SelectionBox = nullptr;
        bBoxIsSpawned = false;
        bMultiplySelectSquadBySelectedBox = false;
    }
}

void ATeamController::OnRightMouseClick(FHitResult Hit)
{
    if (ChoisenSquads.IsEmpty()) return;
    
    OnStopSquad();
    
    if (ChoisenSquads.Num() == 1)
    {
        ChoisenSquads[0]->MoveAndRotatingSquadToLocation(Hit.Location);
    }
    else
    {
        if (SquadGroups.Num() != 0 && ChoisenSquads == SquadGroups[0]->GetSquads())
        {
            int32 Index = 0;
            for (auto& Squad : SquadGroups[0]->GetSquads())
            {
                const FVector NewLocation = Hit.Location + SquadGroups[0]->GetSquadGroupRightCornerLocationsFromCenter()[Index];
                const FVector NewForwardVector = SquadGroups[0]->GetSquadGroupRotationsFromCenter()[Index];
                Squad->UpdateRebuildngSquad(Squad->GetCurrentSquadSizes().Width, NewLocation, NewForwardVector);
                Index++;
            }
        }
        else
        {
            CalcPositionsMoveMultipleSquads(Hit.Location);
        }
        
        for (auto& Squad : ChoisenSquads)
        {
            Squad->EndUpdateRebuildingSquad();
        }
    }
}

void ATeamController::OnRightMouseHold()
{
    if (!bRebuildSquadIsContinue)
    {
        RebuildSquadStartLocation = CameraController->GetMouseLocationOnTerrain();
        bRebuildSquadIsContinue = true;
    }
}

void ATeamController::OnRightMouseHoldCompleted()
{
    if (bRebuildSquadIsContinue)
    {
        bRebuildSquadIsContinue = false;

        if (ChoisenSquads.Num() == 0) return;

        OnStopSquad();
        
        if (ChoisenSquads.Num() == 1)
        {
            ChoisenSquads[0]->EndUpdateRebuildingSquad();
        }
        else
        {
            for (auto& Squad : ChoisenSquads)
            {
                Squad->EndUpdateRebuildingSquad();
            }
        }
    }
}

void ATeamController::AddSquadToChoisenSquadsArray(ABaseSquadCreeps* SquadIn)
{
    if (bMultiplySelectSquadByClick || bMultiplySelectSquadBySelectedBox)
    {
        for (auto& Squad : ChoisenSquads)
        {
            if (Squad.Get() == SquadIn) return;
        }
        ChoisenSquads.Add(SquadIn);
    }
    else
    {
        UnchoiseCurrentSquad();
        ChoisenSquads.Add(SquadIn);
    }
}

void ATeamController::OnSquadIsUnChoisen(ABaseSquadCreeps* SquadIn)
{
    ChoisenSquads.RemoveSingle(SquadIn);
}

void ATeamController::OnMultiplySelectSquad(bool Value)
{
    bMultiplySelectSquadByClick = Value;
}

void ATeamController::OnStopSquad()
{
    if (ChoisenSquads.Num() == 0) return;

    for (auto& Squad : ChoisenSquads)
    {
        Squad->StopAllMovement();
    }
}

void ATeamController::OnGroupingSquad()
{
    if (ChoisenSquads.IsEmpty() || ChoisenSquads.Num() == 1) return;
    
    SquadGroups.Add(NewObject<USquadsGroup>(this));
    SquadGroups[0]->AddSquad(ChoisenSquads);
    
}

void ATeamController::GetSquadsOnLevel()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseSquadCreeps::StaticClass(), Actors);

    if (Actors.IsEmpty()) return;

    for (auto& Actor : Actors)
    {
        auto Squad = Cast<ABaseSquadCreeps>(Actor);
        if (IsValid(Squad))
        {
            SquadsOnLevel.Add(Squad);
        }
    }
}

void ATeamController::BindOnSquadIsChoisenDelegate()
{
    if (SquadsOnLevel.IsEmpty()) return;

    for (auto& Squad : SquadsOnLevel)
    {
        Squad->OnSquadIsUnChoisenBySelectionBox.BindUObject(this, &ATeamController::OnSquadIsUnChoisen);
    }
}

void ATeamController::UnchoiseCurrentSquad()
{
    if (ChoisenSquads.Num() == 0) return;

    for (auto& Squad : ChoisenSquads)
    {
        Squad->SquadUnChoisen();
    }
    ChoisenSquads.Empty(20);
}

void ATeamController::CreateSelectionBox()
{
    if (!GetWorld()) return;

    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    SpawnInfo.Owner = this;
    SelectionBoxStartLocation = CameraController->GetMouseLocationOnTerrain();

    SelectionBox =
        GetWorld()->SpawnActor<ASelectionBox>(SelectionBoxDefaultClass, SelectionBoxStartLocation, FRotator::ZeroRotator, SpawnInfo);
}

void ATeamController::UpdateRebuildSquad()  
{  
    if (ChoisenSquads.IsEmpty()) return;  
    
    FVector EndPoint = CameraController->GetMouseLocationOnTerrain();  
     
    //DrawDebugRebuildLines(EndPoint);  
    
    if (ChoisenSquads.Num() == 1)  
    {  
        RebuildSingleSquad(EndPoint);  
    }  
    else   
    {  
        RebuildMultipleSquads(EndPoint);  
    }  
}  

void ATeamController::DrawDebugRebuildLines(const FVector& EndPoint)  
{  
    DrawDebugLine(GetWorld(), RebuildSquadStartLocation, EndPoint, FColor::Cyan, false, 0, 0u, 20.f);  
    FVector StartPerpendicularLine = FMath::Lerp(RebuildSquadStartLocation, EndPoint, 0.5);  
    FVector RebuildForwardVector = CalculateRebuildForwardVector(EndPoint);  
    DrawDebugLine(GetWorld(), StartPerpendicularLine, StartPerpendicularLine + RebuildForwardVector * 1000.0,  FColor::Cyan, false, 0, 0u, 20.f  );  
}  

void ATeamController::RebuildSingleSquad(const FVector& EndPoint)  
{ 
    FVector RebuildForwardVector = CalculateRebuildForwardVector(EndPoint);  
    double RebuildVectorLength = (EndPoint - RebuildSquadStartLocation).Size();  
    int32 NewWidth = CalculateNewWidthForSingleSquad(RebuildVectorLength);  
    ChoisenSquads[0]->UpdateRebuildngSquad(NewWidth, EndPoint, RebuildForwardVector);  
}  

int32 ATeamController::CalculateNewWidthForSingleSquad(double LengthRebuildVector)  
{  
    FCreepsOffsetInSquad CreepsOffsetInSquad = ChoisenSquads[0]->GetCreepsOffsetInSquad();  
    int32 LengthIntRebuildVector = static_cast<int32>(LengthRebuildVector);  
    int32 OffsetWidthIntInSquad = static_cast<int32>(CreepsOffsetInSquad.Y);  
    
    return FMath::Clamp(LengthIntRebuildVector / OffsetWidthIntInSquad + 1, RebuidSquadClampWidth.Min, ChoisenSquads[0]->GetCreepsNum() / 2);  
}  

void ATeamController::RebuildMultipleSquads(const FVector& EndPoint)  
{  
    FVector RebuildForwardVector = CalculateRebuildForwardVector(EndPoint);  
    float RebuildVectorLength = (EndPoint - RebuildSquadStartLocation).Size();  
    TArray<TObjectPtr<ABaseSquadCreeps>> SquadsFromEndPoint = CalculateSquadsFromEndPoint(EndPoint);  
    float MinLengthOnRebuildAllSquads = CalculateTotalMinRebuildLength(SquadsFromEndPoint);
    FVector RebuildVector = (RebuildSquadStartLocation - EndPoint).GetSafeNormal2D();  
    if (RebuildVectorLength > MinLengthOnRebuildAllSquads)  
    {  
        int32 RebuildSquadsWidth = CalculateMultiSquadWidth(RebuildVectorLength, MinLengthOnRebuildAllSquads);  
        RebuildSquadsPositions(SquadsFromEndPoint, EndPoint, RebuildForwardVector, RebuildVector, RebuildSquadsWidth, true);  
    }  
}  

float ATeamController::CalculateTotalMinRebuildLength(const TArray<TObjectPtr<ABaseSquadCreeps>>& Squads)  
{  
    float TotalLength = 0.0f;  
    for (auto& Squad : Squads)  
    {  
        TotalLength += Squad->GetCreepsOffsetInSquad().Y * RebuidSquadClampWidth.Min + WidthOffsetsOnMultSquadRebuild;  
    }  
    return TotalLength;  
}  

int32 ATeamController::CalculateMultiSquadWidth(float RebuildVectorLength, float MinLengthOnRebuildAllSquads)  
{  
    const int32 AverageOneColumnWidthRise = static_cast<int32>(  
        (MinLengthOnRebuildAllSquads / (RebuidSquadClampWidth.Min * ChoisenSquads.Num())) * ChoisenSquads.Num()  
    );  
    const int32 RebuildVectorLengthInt = static_cast<int32>(RebuildVectorLength);  
    return FMath::Clamp(RebuildVectorLengthInt / AverageOneColumnWidthRise, RebuidSquadClampWidth.Min,100);  
}  

void ATeamController::RebuildSquadsPositions(const TArray<TObjectPtr<ABaseSquadCreeps>>& Squads, const FVector& EndPoint, const FVector& RebuildForwardVector,
    FVector RebuildVector, int32 RebuildSquadsWidth, bool ChangeSquadWidth)  
{  
    double RebuildSquadLength = 0.0f;  
    for (auto& Squad : Squads)  
    {
        if (!IsValid(Squad)) return;
        
        int32 SquadWidth = ChangeSquadWidth ? RebuildSquadsWidth : Squad->GetCurrentSquadSizes().Width;
        FVector RebuildStartLocation = EndPoint + RebuildVector * RebuildSquadLength;  
        Squad->UpdateRebuildngSquad(SquadWidth, RebuildStartLocation, RebuildForwardVector);  
        RebuildSquadLength += Squad->GetCreepsOffsetInSquad().Y * SquadWidth + WidthOffsetsOnMultSquadRebuild;  
    }  
}

void ATeamController::CalcPositionsMoveMultipleSquads(const FVector& HitLocation)
{
    TArray<TObjectPtr<ABaseSquadCreeps>> Squads = CalculateSquadsPositionOrder(true);
    FVector CentralLocation = FMath::Lerp(Squads[Squads.Num() - 1]->GetActorLocation(), Squads[0]->GetActorLocation(), 0.5f);
    FVector ToNewLocationVectorNorm = (HitLocation - CentralLocation).GetSafeNormal2D();
    
    if (Squads[0]->GetActorForwardVector().Dot(ToNewLocationVectorNorm)  < 0.0)
    {
        Squads.Empty();
        Squads = CalculateSquadsPositionOrder(false);
    }
    
    FVector PerpendicularToNewLocationVector(ToNewLocationVectorNorm.Y * -1.0, ToNewLocationVectorNorm.X, ToNewLocationVectorNorm.Z);
    double Length = CalculateMultipleSquadMoveFrontLength(Squads);
    FVector StartLocation = HitLocation + PerpendicularToNewLocationVector * (Length / 2.0);
    RebuildSquadsPositions(Squads, StartLocation, ToNewLocationVectorNorm, PerpendicularToNewLocationVector * -1.0, 0, false);
}

FVector ATeamController::CalculateRebuildForwardVector(FVector EndPoint) const
{
    FVector RebuildVectorNormalize =  (EndPoint - RebuildSquadStartLocation).GetSafeNormal2D();
    FVector PerpendicularRebuildVector(RebuildVectorNormalize.Y * -1.0, RebuildVectorNormalize.X, RebuildVectorNormalize.Z);
    return PerpendicularRebuildVector * -1.0;
}

TArray<TObjectPtr<ABaseSquadCreeps>> ATeamController::CalculateSquadsFromEndPoint(FVector EndPoint) const
{
    TArray<TObjectPtr<ABaseSquadCreeps>> Result{};

    for (auto& Squad : ChoisenSquads)
    {
        Result.Add(Squad);
    }
    Result.Sort([&](const TObjectPtr<ABaseSquadCreeps>& Left, const TObjectPtr<ABaseSquadCreeps>& Right)
    {
        const double LeftLength = (EndPoint - Left->GetActorLocation()).Size2D();
        const double RightLength = (EndPoint - Right->GetActorLocation()).Size2D();
        return LeftLength < RightLength;
    });
    
    return Result;
}

TArray<TObjectPtr<ABaseSquadCreeps>> ATeamController::CalculateSquadsPositionOrder(bool RightToLeft)
{
    TArray<TObjectPtr<ABaseSquadCreeps>> Result{};
    for (auto& Squad : ChoisenSquads)
    {
        Result.Add(Squad);
    }
    
    Result.Sort([&](const TObjectPtr<ABaseSquadCreeps>& First, const TObjectPtr<ABaseSquadCreeps>& Second)
    {
        FVector FirstRightVector = First->GetActorRightVector();
        FVector FirstToSecondVectorNorm = (Second->GetActorLocation() - First->GetActorLocation()).GetSafeNormal2D();
        float DotResult = FirstRightVector.Dot(FirstToSecondVectorNorm);
        return RightToLeft ? (DotResult < 0.0f) : (DotResult > 0.0f);
    });
    return Result;
}


FVector ATeamController::CalculatePositionCentralSquad(const TArray<TObjectPtr<ABaseSquadCreeps>>& SquadsFromHitLocation)
{
    if (SquadsFromHitLocation.Num() == 0) return FVector::Zero();

    if (SquadsFromHitLocation.Num() == 1)
    {
        return SquadsFromHitLocation[0]->GetActorLocation();
    }
    else
    {
        int32 CentralSquadIndex = SquadsFromHitLocation.Num() / 2;
        return SquadsFromHitLocation[CentralSquadIndex]->GetActorLocation();
    }
}

double ATeamController::CalculateMultipleSquadMoveFrontLength(const TArray<TObjectPtr<ABaseSquadCreeps>>& SquadsFromHitLocation)
{
    if (SquadsFromHitLocation.Num() == 0 || SquadsFromHitLocation.Num() == 1 ) return 0.0;
    double Result = 0.0;
    for (auto& Squad : SquadsFromHitLocation)
    {
        Result += Squad->GetCreepsOffsetInSquad().Y * Squad->GetCurrentSquadSizes().Width + WidthOffsetsOnMultSquadRebuild;
    }
    return Result;
}