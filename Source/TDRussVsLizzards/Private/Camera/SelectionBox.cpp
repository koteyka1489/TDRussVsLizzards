// TD Russ Vs Lizzards Game

#include "Camera/SelectionBox.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASelectionBox::ASelectionBox()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    SetRootComponent(BoxCollider);
    BoxCollider->SetBoxExtent(FVector(1.0, 1.0, 1.0));
    BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
    BoxCollider->SetVisibility(true);
    BoxCollider->bHiddenInGame = false;

    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComponent->SetupAttachment(GetRootComponent());
    DecalComponent->SetVisibility(false);
    DecalComponent->DecalSize = FVector::Zero();
}

void ASelectionBox::BeginPlay()
{
    Super::BeginPlay();

    StartLocation   = GetActorLocation();
    StartLocation.Z = 0.0;

    BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASelectionBox::OnBoxColliderBeginOverlap);
}

void ASelectionBox::Update(FVector MouseLocation) 
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

void ASelectionBox::SelectionComplete() 
{
    this->Destroy();
}


void ASelectionBox::OnBoxColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}