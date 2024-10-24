// TD Russ Vs Lizzards Game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectionBox.generated.h"

class UBoxComponent;
class UDecalComponent;
class ABaseCreepActor;
class ABaseSquadCreeps;

UCLASS()
class TDRUSSVSLIZZARDS_API ASelectionBox : public AActor
{
    GENERATED_BODY()

public:
    ASelectionBox();
    virtual void BeginPlay() override;

    void Update(FVector MouseLocation);

    void SelectionComplete();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UBoxComponent* BoxCollider;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UDecalComponent* DecalComponent;

    UFUNCTION()
    void OnBoxColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
    FVector StartLocation = FVector::Zero();

    TArray<TObjectPtr<ABaseSquadCreeps>> SelectedSquads;

    void UpdateBoxSizesAndLocation(FVector MouseLocation);
    void UpdateSelectingChoisedSquad();
};
