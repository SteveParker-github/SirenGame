// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CheckpointTrigger.generated.h"

UCLASS()
class SIRENS_CALL_API ACheckpointTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpointTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
	void OnOverlapBegin(
		class UPrimitiveComponent *OverlappedComp,
		class AActor *OtherActor,
		class UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
	int32 ZoneLevel;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerZone;
};
