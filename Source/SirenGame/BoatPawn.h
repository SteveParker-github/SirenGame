// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoatPawn.generated.h"


class AMouseCursor;

UCLASS()
class SIRENGAME_API ABoatPawn : public APawn
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

	UPROPERTY(VisibleAnywhere)
	float Health;

	float WaterDirectionTimer = 0;

	UPROPERTY(EditDefaultsOnly, Category = WaterFlow)
	float WaterDirectionCooldown = 2;

	UPROPERTY(EditDefaultsOnly, Category = WaterFlow)
	float Force = 1385;

	TArray<AActor *> WaterFlows;

	FVector TargetLocation;

	UPROPERTY()
	AActor* TargetWaterFlow;

	UPROPERTY()
	FVector WaterFlowDirection;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoatMesh;
};
