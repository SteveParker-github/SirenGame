// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "BoatPawn.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->SetupAttachment(Mesh);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(Root);
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = false;
	}

	InitialLifeSpan = 4.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::OnOverlapBegin(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	APawn* Boat = Cast<APawn>(OtherActor);
	if (Boat == nullptr) return;
	FPointDamageEvent DamageEvent(Damage, SweepResult, GetActorForwardVector(), nullptr);
	AController* BoatController = Boat->GetController();
	Boat->TakeDamage(Damage, DamageEvent, BoatController, this);
	this->Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Function that initializes the projectile's velocity in the shoot direction.
void AProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
