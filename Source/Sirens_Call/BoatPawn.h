// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoatPawn.generated.h"


class AMouseCursor;

UCLASS()
class SIRENS_CALL_API ABoatPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoatPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void SetSirenZone(FVector SirenLocation);

	void UnsetSirenZone();

	UPROPERTY(VisibleAnywhere)
	float BoatHealth;

	UPROPERTY(VisibleAnywhere)
	float CrewHealth;

private:
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveControllerForward(float AxisValue);
	void MoveControllerRight(float AxisValue);	
	void InitiateAim();
	void SetAim();
	void UnsetAim();
	void Fire();

	void UpdateWaterFlow();
	void NewWaterDirection();
	void MoveTowardsSiren();

	bool bAiming;
	
	FVector MovementInput;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 1;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMouseCursor> MouseCursorClass;

	UPROPERTY()
	AMouseCursor* MouseCursor;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	float WaterDirectionTimer = 0;

	UPROPERTY(EditDefaultsOnly, Category = WaterFlow)
	float WaterDirectionCooldown = 2;

	UPROPERTY(EditDefaultsOnly, Category = WaterFlow)
	float Force = 0.4;

	UPROPERTY(EditDefaultsOnly, Category = Siren)
	float SirenForce = 1385;

	TArray<AActor *> WaterFlows;

	FVector TargetLocation;

	AActor* TargetWaterFlow;

	FVector WaterFlowDirection;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoatMesh;

	FVector SirenLocation;
	
	bool bInSirenZone;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMatineeCameraShake> CamShake;

	UFUNCTION()    
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
