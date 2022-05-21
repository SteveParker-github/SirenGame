// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "FP_Character.generated.h"

class ABoatPawn;

UCLASS()
class SIRENS_CALL_API AFP_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFP_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnOverlapBegin(
		class UPrimitiveComponent *OverlappedComp,
		class AActor *OtherActor,
		class UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		class UPrimitiveComponent *OverlappedComp,
		class AActor *OtherActor,
		class UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex);

	bool bIsCurrentlyPossessed;

	bool bCanSwap;

private:
	void FPMoveForward(float AxisValue);
	void FPMoveRight(float AxisValue);
	void SwapCharacter();

	AController* SavedController;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerZone;

	ABoatPawn* BoatPawn;
};
