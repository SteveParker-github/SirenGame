// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseCursor.h"

// Sets default values
AMouseCursor::AMouseCursor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AMouseCursor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMouseCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMouseCursor::MoveCursor(FVector NewLocation)
{
	SetActorLocation(NewLocation);
}

