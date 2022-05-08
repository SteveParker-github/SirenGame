// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FP_Hero.generated.h"

UCLASS()
class SIRENS_CALL_API AFP_Hero : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFP_Hero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void HeroForward(float AxisValue);
	void HeroLookUp(float AxisValue);
	void HeroRight(float AxisValue);
	void HeroLookRight(float AxisValue);

};
