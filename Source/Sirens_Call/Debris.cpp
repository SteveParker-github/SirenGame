// Fill out your copyright notice in the Description page of Project Settings.


#include "Debris.h"
#include "Kismet/GameplayStatics.h"
#include "WaterFlow.h"
#include "Components/SplineComponent.h"

// Sets default values
ADebris::ADebris()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DebrisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DebrisMesh"));
	SetRootComponent(DebrisMesh);

}

// Called when the game starts or when spawned
void ADebris::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterFlow::StaticClass(), WaterFlows);
}

// Called every frame
void ADebris::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WaterDirectionTimer -= DeltaTime;

	UpdateWaterFlow();
}

void ADebris::UpdateWaterFlow()
{
	if (WaterDirectionTimer <= 0)
	{
		NewWaterDirection();
		WaterDirectionTimer = WaterDirectionCooldown;
	}

	if (TargetWaterFlow != nullptr)
	{
		USplineComponent *Spline = Cast<USplineComponent>(TargetWaterFlow->GetRootComponent());

		if (Spline == nullptr)
		{
			return;
		}

		WaterFlowDirection = Spline->FindDirectionClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
		WaterFlowDirection.Z = -2;
		SetActorLocation(GetActorLocation() + WaterFlowDirection);

	}	
}

void ADebris::NewWaterDirection()
{
	float ClosestDistance = FLT_MAX;
	FVector DebrisLocation = GetActorLocation();

	for (AActor *WaterFlow : WaterFlows)
	{
		USplineComponent *Spline = Cast<USplineComponent>(WaterFlow->GetRootComponent());
		if (Spline == nullptr)
			return;

		FVector Location = Spline->FindLocationClosestToWorldLocation(DebrisLocation, ESplineCoordinateSpace::World);
		float CheckDistance = DebrisLocation.Distance(DebrisLocation, Location);
		if (CheckDistance < ClosestDistance)
		{
			ClosestDistance = CheckDistance;
			TargetWaterFlow = WaterFlow;
		}
	}
}

