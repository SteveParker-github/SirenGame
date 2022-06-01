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
#include "FP_Character.h"
#include "InGameHUD.h"

// Sets default values
ABoatPawn::ABoatPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoatMesh"));
	SetRootComponent(BoatMesh);

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	TriggerZone->SetupAttachment(BoatMesh);

	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ABoatPawn::OnOverlapBegin);

	bIsCurrentlyPossessed = false;

	MaxBoatHealth = MaxStartingHealth;
	MaxCrewHealth = MaxStartingHealth;
	MaxArcherHealth = MaxStartingHealth;
	MaxStamina = MaxStartingHealth;
	BoatHealth = MaxBoatHealth;
	CrewHealth = MaxCrewHealth;
	ArcherHealth = MaxArcherHealth;
	Stamina = MaxStamina;

	bInSirenZone = false;
	bCanSwap = false;
	bIsPowerRow = false;
	CurrentCheckpoint = 1;
}

// Called when the game starts or when spawned
void ABoatPawn::BeginPlay()
{
	Super::BeginPlay();

	AInGameHUD *InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (InGameHUD)
	{
		InGameHUD->UpdateCrewHealth(CrewHealth / MaxCrewHealth);
		InGameHUD->UpdateBoatHealth(BoatHealth / MaxBoatHealth);
		InGameHUD->UpdateArcherHealth(ArcherHealth / MaxArcherHealth);
		InGameHUD->UpdateStamina(Stamina / MaxStamina);
	}
	MouseCursor = GetWorld()->SpawnActor<AMouseCursor>(MouseCursorClass);
	MouseCursor->SetOwner(this);

	BoatMesh->OnComponentHit.AddDynamic(this, &ABoatPawn::OnHit);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterFlow::StaticClass(), WaterFlows);

	FPCharacter = Cast<AFP_Character>(UGameplayStatics::GetActorOfClass(GetWorld(), AFP_Character::StaticClass()));
}

// Called every frame
void ABoatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsCurrentlyPossessed)
	{
		return;
	}

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
	Direction.Z = 0;

	if (bIsPowerRow)
	{
		if (Stamina > 0)
		{
			BoatMesh->AddForce(GetActorForwardVector() * 1500 * BoatMesh->GetMass());
			Stamina -= DeltaTime * 20;
			AInGameHUD *InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			InGameHUD->UpdateStamina(Stamina / MaxStamina);
		}
	}
	else
	{
		AInGameHUD *InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		Stamina = FMath::Min(Stamina + DeltaTime * 2, MaxStamina);
		InGameHUD->UpdateStamina(Stamina / MaxStamina);
	}
	AddMovementInput(Direction);

	if (bInSirenZone)
	{
		float Damage = DeltaTime * 10; // Causes 1 damge per second.
		CrewHealth -= Damage;
		ArcherHealth -= Damage;
		AInGameHUD *InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

		if (InGameHUD)
		{
			InGameHUD->UpdateCrewHealth(CrewHealth / MaxCrewHealth);
			InGameHUD->UpdateArcherHealth(ArcherHealth / MaxArcherHealth);
		}
		MoveTowardsSiren();
	}
	else
	{
		UpdateWaterFlow();
	}

	if (IsDead())
	{
		RemoveHUD();
		AInGameHUD *InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (InGameHUD)
		{
			InGameHUD->DeathHUD(CurrentCheckpoint);
		}
		bCanSwap = false;
		SavedController = GetController();
		SavedController->UnPossess();
		this->Destroy();
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
	PlayerInputComponent->BindAction(TEXT("Activate"), EInputEvent::IE_Pressed, this, &ABoatPawn::SwapCharacter);
	PlayerInputComponent->BindAction(TEXT("PowerRow"), EInputEvent::IE_Pressed, this, &ABoatPawn::SetPowerRow);
	PlayerInputComponent->BindAction(TEXT("PowerRow"), EInputEvent::IE_Released, this, &ABoatPawn::UnsetPowerRow);
}

float ABoatPawn::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(BoatHealth, DamageToApply);
	BoatHealth -= DamageToApply;

	AInGameHUD *InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (InGameHUD)
	{
		InGameHUD->UpdateBoatHealth(BoatHealth / MaxBoatHealth);
	}

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

void ABoatPawn::OnOverlapBegin(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (bIsCurrentlyPossessed)
	{
		bCanSwap = true;
	}
}

void ABoatPawn::OnOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (bIsCurrentlyPossessed)
	{
		bCanSwap = false;
	}
}

void ABoatPawn::RemoveHUD()
{
	AInGameHUD *InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (InGameHUD)
	{
		InGameHUD->RemoveHUD();
	}
}

void ABoatPawn::AddHUD()
{
	AInGameHUD *InGameHUD = Cast<AInGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (InGameHUD)
	{
		InGameHUD->AddHUD();
	}
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
		NewLocation = NewLocation.GetClampedToSize(-200, 2000);
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
		ArrowLocation.Z += 50.f;
		FRotator ArrowDirection = (MouseLocation - ActorLocation).Rotation();
		float TargetDistance = ArrowLocation.Distance(MouseLocation, ActorLocation);
		// Parabola/hyperbola graph
		// ArrowDirection.Pitch = -2.6016239843361 * FMath::Pow(10, -6) * FMath::Pow(TargetDistance, 2) - 0.0104899 * TargetDistance + 89.7078;
		ArrowDirection.Pitch = 0;
		if (FMath::Abs(TargetDistance) > 500)
		{
			ArrowDirection.Pitch = 1.333 * FMath::Pow(10, -8) * FMath::Pow(TargetDistance, 3) - 4 * FMath::Pow(10, -5) * FMath::Pow(TargetDistance, 2) + 0.057 * TargetDistance - 15;
		}

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

void ABoatPawn::SwapCharacter()
{
	if (bCanSwap)
	{
		SavedController = GetController();
		SavedController->UnPossess();
		SavedController->Possess(Cast<APawn>(FPCharacter));
		bIsCurrentlyPossessed = false;
		FPCharacter->bIsCurrentlyPossessed = true;
		FPCharacter->bCanSwap = false;
		RemoveHUD();
		bCanSwap = false;
	}
}

void ABoatPawn::SetPowerRow()
{
	bIsPowerRow = true;
}

void ABoatPawn::UnsetPowerRow()
{
	bIsPowerRow = false;
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
			if (WindEffectTimer == 0)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WindEffect, GetActorLocation(), WaterFlowDirection.Rotation());
				WindEffectTimer = 30;
			}
			--WindEffectTimer;
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
	// BoatMesh->AddForce(SirenHeading * SirenForce * BoatMesh->GetMass());
	AddMovementInput(SirenHeading * SirenForce, 10, true);
	if (WindEffectTimer == 0)
	{
		FRotator test = SirenHeading.Rotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WindEffect, GetActorLocation(), SirenHeading.Rotation());
		WindEffectTimer = 30;
	}
	--WindEffectTimer;
}

bool ABoatPawn::IsDead()
{
	return CrewHealth <= 0 || BoatHealth <= 0;
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