// TD Russ Vs Lizzards Game

#include "Camera/SelectionBox.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

ASelectionBox::ASelectionBox()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    SetRootComponent(BoxCollider);
    BoxCollider->SetBoxExtent(FVector(1.0, 1.0, 1.0));
    BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxCollider->SetCollisionResponseToAllChannels(ECR_Overlap);

    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
    DecalComponent->SetupAttachment(GetRootComponent());
}

void ASelectionBox::BeginPlay()
{
    Super::BeginPlay();

    BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ASelectionBox::OnBoxColliderBeginOverlap);
}

void ASelectionBox::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASelectionBox::OnBoxColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}