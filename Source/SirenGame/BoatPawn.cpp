// Fill out your copyright notice in the Description page of Project Settings.

#include "BoatPawn.h"

#include "MouseCursor.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "GenericPlatform/GenericPlatformMath.h"

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

	Health = MaxHealth;

	MouseCursor = GetWorld()->SpawnActor<AMouseCursor>(MouseCursorClass);
	MouseCursor->SetOwner(this);
}

// Called every frame
void ABoatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bAiming)
	{
		InitiateAim();
	}

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
void ABoatPawn::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABoatPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABoatPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveControllerForward"), this, &ABoatPawn::MoveControllerForward);
	PlayerInputComponent->BindAxis(TEXT("MoveControllerRight"), this, &ABoatPawn::MoveControllerRight);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &ABoatPawn::SetAim);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &ABoatPawn::UnsetAim);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ABoatPawn::Fire);
}

float ABoatPawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
	return DamageToApply;
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

void ABoatPawn::InitiateAim()
{
	APlayerController *OwnerPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (OwnerPlayerController == nullptr)
		return;

	FVector Location;
	FVector Direction;
	FVector DummyVector;
	FRotator DummyRotator;

	OwnerPlayerController->DeprojectMousePositionToWorld(DummyVector, Direction);

	Direction *= 7000.0f;
	OwnerPlayerController->GetPlayerViewPoint(Location, DummyRotator);

	FVector EndVector = Direction + Location;

	// DrawDebugLine(GetWorld(), Location, EndVector, FColor::Red, true);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	FHitResult Hit;
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, EndVector, ECollisionChannel::ECC_GameTraceChannel1, Params);

	if (bSuccess)
	{
		FVector ActorLocation = GetActorLocation();
		FVector NewLocation = Hit.Location - ActorLocation;
		NewLocation = NewLocation.GetClampedToSize(-2000, 2000);
		NewLocation += ActorLocation;
		NewLocation.Z = -39;
		MouseCursor->MoveCursor(NewLocation);
		FVector HalfWay = ((NewLocation - ActorLocation) / 2 + ActorLocation);
		float Distance = HalfWay.Distance(NewLocation, ActorLocation);
		FVector Angle = ActorLocation - NewLocation;
		Angle = Angle.GetClampedToSize(-1, 1);
		// DrawDebugCircle(GetWorld(), HalfWay, Distance / 2, 50, FColor::Red, false, -1.f, 0, 0.f, Angle, FVector(0.f, 0.f, 1.f), false);
	}
}

void ABoatPawn::SetAim()
{
	MouseCursor->SetActorHiddenInGame(false);
	bAiming = true;
}

void ABoatPawn::UnsetAim()
{
	MouseCursor->SetActorHiddenInGame(true);
	bAiming = false;
}

void ABoatPawn::Fire()
{
	if (bAiming)
	{
		FVector ActorLocation = GetActorLocation();
		FVector MouseLocation = MouseCursor->GetActorLocation();
		FVector ArrowLocation = ActorLocation;
		ArrowLocation.Z += 200.f;
		FRotator ArrowDirection = (MouseLocation - ActorLocation).Rotation();
		float TargetDistance = ArrowLocation.Distance(MouseLocation, ActorLocation);
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), TargetDistance);
		// calculated from power
		// ArrowDirection.Pitch = 89.513 - 0.00245295 * FMath::Pow(TargetDistance, 1.24239);
		// Parabola/hyperbola graph
		ArrowDirection.Pitch = -2.6016239843361 * FMath::Pow(10, -6) * FMath::Pow(TargetDistance, 2) - 0.0104899 * TargetDistance + 89.7078;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		for (int32 i = 0; i < 20; i++)
		{
			FVector SpawnLocation = ArrowLocation;
			FRotator RandomDirection = ArrowDirection;
			SpawnLocation.X += FMath::RandRange(-50, 50);
			SpawnLocation.Y += FMath::RandRange(-50, 50);
			RandomDirection += FRotator(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), FMath::RandRange(-1, 1));
			AProjectile *Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, RandomDirection, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = RandomDirection.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}