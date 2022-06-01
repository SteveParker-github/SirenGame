// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointTrigger.h"
#include "BoatPawn.h"

// Sets default values
ACheckpointTrigger::ACheckpointTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	SetRootComponent(TriggerZone);
}

// Called when the game starts or when spawned
void ACheckpointTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ACheckpointTrigger::OnOverlapBegin);	
}

// Called every frame
void ACheckpointTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpointTrigger::OnOverlapBegin(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    ABoatPawn* Boat = Cast<ABoatPawn>(OtherActor);
    if (Boat == nullptr) return;
    Boat->CurrentCheckpoint = ZoneLevel;
}
