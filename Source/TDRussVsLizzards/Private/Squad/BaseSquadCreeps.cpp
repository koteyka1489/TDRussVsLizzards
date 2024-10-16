// TD Russ Vs Lizzards Game

#include "Squad/BaseSquadCreeps.h"
#include "Creeps/BaseCreepActor.h"
#include "Creeps/OrcCreepActor.h"
#include "Creeps/TrollCreepActor.h"
#include "Creeps/BarbarianCreepActor.h"
#include "Components/InstancedStaticMeshComponent.h"

ABaseSquadCreeps::ABaseSquadCreeps()
{
    PrimaryActorTick.bCanEverTick = true;

    SelectionInstancedMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("UInstancedStaticMeshComponent"));
     

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SelectCircleMesh(
        TEXT("/Script/Engine.StaticMesh'/Game/Squads/SM_SelectCircle.SM_SelectCircle'"));

    checkf(SelectCircleMesh.Succeeded(), TEXT("Find SelectCircleMesh is not Succeeded "));

    SelectionInstancedMesh->SetStaticMesh(SelectCircleMesh.Object);

    SelectionInstancedMesh->InstancingRandomSeed = (int32)FMath::RandRange(1000.0, 5000.0);
    SelectionInstancedMesh->bDisableCollision    = true;
    SelectionInstancedMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}

void ABaseSquadCreeps::BeginPlay()
{
    Super::BeginPlay();

    Creeps.Reserve(CreepsNum);
    SpawnCreeps();
    BindOnCreepIsClickedtDelegate();


    for (auto& Creep : Creeps)
    {
        SelectionInstancedMesh->AddInstance(Creep->GetActorTransform(), true);
    }
    SelectionInstancedMesh->SetVisibility(false);
    
}

void ABaseSquadCreeps::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseSquadCreeps::SpawnCreeps()
{
    int32 CreepsRows            = FMath::FloorToInt(FMath::Sqrt((float)CreepsNum));
    int32 CreepsColumns         = CreepsRows;
    int32 CreepsRemainder       = CreepsNum - CreepsRows * CreepsColumns;
    const FVector Squadlocation = GetActorLocation();
    FRotator SpawnRotation{0.0, 180.0, 0.0};

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
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Squad is choisen"));

    checkf(OnSquadIsChoisen.ExecuteIfBound(this), TEXT("OnSquadIsChoisen Is not bound"));

    for (auto& Creep : Creeps)
    {
        Creep->SetCreepIsChoisen(true);
    }
    SelectionInstancedMesh->SetVisibility(true);
}

void ABaseSquadCreeps::SquadUnChoisen()
{
    bSquadIsChosen = false;
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Squad is Un choisen"));
    for (auto& Creep : Creeps)
    {
        Creep->SetCreepIsChoisen(false);
    }
    SelectionInstancedMesh->SetVisibility(false);
}
