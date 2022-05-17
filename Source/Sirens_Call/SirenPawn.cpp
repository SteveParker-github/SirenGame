// Fill out your copyright notice in the Description page of Project Settings.


#include "SirenPawn.h"

#include "BoatPawn.h"

// Sets default values
ASirenPawn::ASirenPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	TriggerZone = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerZone"));
	TriggerZone->SetupAttachment(Mesh);
	TriggerZone->SetSphereRadius(2000, true);

	TriggerZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerZoneMesh"));
	TriggerZoneMesh->SetupAttachment(TriggerZone);
	
	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ASirenPawn::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ASirenPawn::BeginPlay()
{
	Super::BeginPlay();

	FVector MoveTrigger = GetActorLocation();
	MoveTrigger.Z = -40;
	TriggerZone->SetWorldLocation(MoveTrigger);

	TriggerZone->OnComponentEndOverlap.AddDynamic(this, &ASirenPawn::OnOverlapEnd);
}

// Called every frame
void ASirenPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASirenPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASirenPawn::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABoatPawn* Pawn = Cast<ABoatPawn>(OtherActor);

	if (Pawn != nullptr)
	{
		Pawn->SetSirenZone(GetActorLocation());
	}
}

void ASirenPawn::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABoatPawn* Pawn = Cast<ABoatPawn>(OtherActor);

	if (Pawn != nullptr)
	{
		Pawn->UnsetSirenZone();
	}
}

