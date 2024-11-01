// TD Russ Vs Lizzards Game

#include "Camera/SelectionBox.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Squad/BaseSquadCreeps.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraPawn.h"

ASelectionBox::ASelectionBox()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    SetRootComponent(BoxCollider);
    BoxCollider->SetBoxExtent(FVector(1.0, 1.0, 1.0));
    BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    BoxCollider->SetVisibility(false);
    BoxCollider->bHiddenInGame = true;

    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComponent->SetupAttachment(GetRootComponent());
    DecalComponent->SetVisibility(false);
    DecalComponent->DecalSize = FVector(1.0, 1.0, 1.0);
}

void ASelectionBox::BeginPlay()
{
    Super::BeginPlay();

    StartLocation   = GetActorLocation();
    StartLocation.Z = 0.0;

    BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASelectionBox::OnBoxColliderBeginOverlap);
    BoxCollider->OnComponentEndOverlap.AddDynamic(this, &ASelectionBox::OnBoxColliderEndOverlap);
}

void ASelectionBox::Update(FVector MouseLocation)
{
    UpdateBoxSizesAndLocation(MouseLocation);
    UpdateSelectingChoisedSquad();
}

void ASelectionBox::SelectionComplete()
{
    SelectedSquads.Empty();
    this->Destroy();
}

void ASelectionBox::OnBoxColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    auto OverlapBox = Cast<UBoxComponent>(OtherComp);
    if (!OverlapBox) return;

    auto OverlapSquad = Cast<ABaseSquadCreeps>(OverlapBox->GetOwner());
    if (!OverlapSquad) return;

    if (SelectedSquads.Num() == 0)
    {
        SelectedSquads.Add(OverlapSquad);
    }
    else
    {
        for (const auto& SelectedSquad : SelectedSquads)
        {
            if (SelectedSquad.Get() == OverlapSquad)
            {
                return;
            }
        }
        SelectedSquads.Add(OverlapSquad);
    }
}

void ASelectionBox::OnBoxColliderEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

    auto OverlapBox = Cast<UBoxComponent>(OtherComp);
    if (!OverlapBox) return;

    auto OverlapSquad = Cast<ABaseSquadCreeps>(OverlapBox->GetOwner());
    if (!IsValid(OverlapSquad)) return;

    if (SelectedSquads.Num() == 0) return;

    SelectedSquads.RemoveSingle(OverlapSquad);

    OverlapSquad->SquadUnChoisenBySelectBox();
}

void ASelectionBox::UpdateBoxSizesAndLocation(FVector MouseLocation)
{
    FVector EndPoint(MouseLocation.X, MouseLocation.Y, 0.0);
    SetActorLocation(UKismetMathLibrary::VLerp(StartLocation, EndPoint, 0.5f));

    FVector NewBoxExtend = GetActorLocation() - EndPoint;
    NewBoxExtend         = NewBoxExtend.GetAbs();
    NewBoxExtend.Z       = 1000.0;

    BoxCollider->SetBoxExtent(NewBoxExtend);

    FVector DecalSize(NewBoxExtend.Z, NewBoxExtend.Y, NewBoxExtend.X);
    DecalComponent->DecalSize = DecalSize;
    DecalComponent->SetVisibility(true);
}

void ASelectionBox::UpdateSelectingChoisedSquad()
{
    if (SelectedSquads.Num() == 0) return;

    for (auto& SelectedSquad : SelectedSquads)
    {
        auto OwnerCameraPawn = Cast<ACameraPawn>(SelectedSquad->GetOwner());

        if (OwnerCameraPawn)
        {
            OwnerCameraPawn->SetSquadIsChoisen(SelectedSquad);
        }
    }
}
