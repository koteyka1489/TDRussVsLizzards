// TD Russ Vs Lizzards Game

#include "Camera/SelectionBox.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

ASelectionBox::ASelectionBox()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    SetRootComponent(BoxCollider);
    BoxCollider->SetBoxExtent(FVector(10.0, 10.0, 10.0));
    BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
    BoxCollider->SetVisibility(true);


    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComponent->SetupAttachment(GetRootComponent());

    StartPosition = GetActorLocation();
}

void ASelectionBox::BeginPlay()
{
    Super::BeginPlay();

    BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASelectionBox::OnBoxColliderBeginOverlap);
}

void ASelectionBox::Update(FVector MouseLocation) 
{

}

void ASelectionBox::SelectionComplete() 
{
    this->Destroy();
}


void ASelectionBox::OnBoxColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}