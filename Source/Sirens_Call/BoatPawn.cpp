// Fill out your copyright notice in the Description page of Project Settings.

#include "BoatPawn.h"

#include "MouseCursor.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "GenericPlatform/GenericPlatformMath.h"

#include "WaterFlow.h"
#include "Components/SplineComponent.h"

// Sets default values
ABoatPawn::ABoatPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoatMesh"));
	SetRootComponent(BoatMesh);
}

// Called when the game starts or when spawned
void ABoatPawn::BeginPlay()
{
	Super::BeginPlay();

	BoatHealth = MaxHealth;
	CrewHealth = MaxHealth;
	bInSirenZone = false;

	MouseCursor = GetWorld()->SpawnActor<AMouseCursor>(MouseCursorClass);
	MouseCursor->SetOwner(this);

	BoatMesh->OnComponentHit.AddDynamic(this, &ABoatPawn::OnHit);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterFlow::StaticClass(), WaterFlows);
}

// Called every frame
void ABoatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	WaterDirectionTimer -= DeltaTime;

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

	FVector Direction = GetActorForwardVector() * MovementInput.Size();
	Direction.Z = -2;
	AddMovementInput(Direction);

	if (bInSirenZone)
	{
		CrewHealth -= DeltaTime * 1; // Causes 1 damge per second.
		MoveTowardsSiren();
	}
	else
	{
		UpdateWaterFlow();
	}
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

float ABoatPawn::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(BoatHealth, DamageToApply);
	BoatHealth -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), BoatHealth);
	return DamageToApply;
}

void ABoatPawn::SetSirenZone(FVector NewSirenLocation)
{
	SirenLocation = NewSirenLocation;
	bInSirenZone = true;
}

void ABoatPawn::UnsetSirenZone()
{
	bInSirenZone = false;
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
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, EndVector, ECollisionChannel::ECC_GameTraceChannel2, Params);

	if (bSuccess)
	{
		FVector ActorLocation = GetActorLocation();
		FVector NewLocation = Hit.Location - ActorLocation;
		NewLocation = NewLocation.GetClampedToSize(-2000, 2000);
		NewLocation += ActorLocation;
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

void ABoatPawn::UpdateWaterFlow()
{
	if (WaterDirectionTimer <= 0)
	{
		NewWaterDirection();
		WaterDirectionTimer = WaterDirectionCooldown;
	}

	if (TargetWaterFlow != nullptr)
	{
		USplineComponent *Spline = Cast<USplineComponent>(TargetWaterFlow->GetRootComponent());

		if (Spline != nullptr)
		{
			WaterFlowDirection = Spline->FindDirectionClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
			WaterFlowDirection.Z = 0;
			AddMovementInput(WaterFlowDirection, Force, true);
		}
	}
}

void ABoatPawn::NewWaterDirection()
{
	float ClosestDistance = FLT_MAX;
	FVector BoatLocation = GetActorLocation();

	for (AActor *WaterFlow : WaterFlows)
	{
		USplineComponent *Spline = Cast<USplineComponent>(WaterFlow->GetRootComponent());
		if (Spline == nullptr)
			return;

		FVector Location = Spline->FindLocationClosestToWorldLocation(BoatLocation, ESplineCoordinateSpace::World);
		float CheckDistance = BoatLocation.Distance(BoatLocation, Location);
		if (CheckDistance < ClosestDistance)
		{
			ClosestDistance = CheckDistance;
			TargetWaterFlow = WaterFlow;
		}
	}
}

void ABoatPawn::MoveTowardsSiren()
{
	FVector SirenHeading = SirenLocation - GetActorLocation();
	SirenHeading.Z = 0;
	SirenHeading = SirenHeading.GetClampedToSize(-1, 1);
	BoatMesh->AddForce(SirenHeading * SirenForce * BoatMesh->GetMass());
}

void ABoatPawn::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if (OtherActor->GetName() == "Landscape_1")
	{
		FVector PushDirection = this->GetVelocity() * -1;

		PushDirection.Z = 0;

		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CamShake, 1);
		AddMovementInput(PushDirection, 10, true);
	}
}