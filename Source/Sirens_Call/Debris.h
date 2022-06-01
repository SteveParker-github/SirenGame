// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Debris.generated.h"

UCLASS()
class SIRENS_CALL_API ADebris : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebris();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void UpdateWaterFlow();
	void NewWaterDirection();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *DebrisMesh;

	float WaterDirectionTimer = 0;

	UPROPERTY(EditDefaultsOnly, Category = WaterFlow)
	float WaterDirectionCooldown = 2;

	UPROPERTY(EditDefaultsOnly, Category = WaterFlow)
	float Force = 0.4;

	TArray<AActor *> WaterFlows;

	FVector TargetLocation;

	AActor *TargetWaterFlow;

	FVector WaterFlowDirection;

};
