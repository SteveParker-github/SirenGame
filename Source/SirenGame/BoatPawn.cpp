// Fill out your copyright notice in the Description page of Project Settings.


#include "BoatPawn.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ABoatPawn::ABoatPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoatPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementInput.Size() > 0.25)
	{
		FRotator TargetRotation = MovementInput.Rotation();
		
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);
		FQuat QuatRotation = FQuat(NewRotation);
		SetActorRotation(NewRotation, ETeleportType::None);
	}

	AddMovementInput(GetActorForwardVector() * MovementInput.Size());
}

// Called to bind functionality to input
void ABoatPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABoatPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABoatPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveControllerForward"), this, &ABoatPawn::MoveControllerForward);
	PlayerInputComponent->BindAxis(TEXT("MoveControllerRight"), this, &ABoatPawn::MoveControllerRight);
}

void ABoatPawn::MoveForward(float AxisValue)
{
	
	FVector Move = GetActorForwardVector() * AxisValue;
	AddMovementInput(Move);
}

void ABoatPawn::MoveRight(float AxisValue)
{
	FRotator NewRotation = FRotator(0, RotationSpeed * AxisValue, 0);

	FQuat QuatRotation = FQuat(NewRotation);

	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

void ABoatPawn::MoveControllerForward(float AxisValue)
{
	MovementInput.X = AxisValue;
}

void ABoatPawn::MoveControllerRight(float AxisValue)
{
	MovementInput.Y = AxisValue;
}