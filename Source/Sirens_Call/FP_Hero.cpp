// Fill out your copyright notice in the Description page of Project Settings.


#include "FP_Hero.h"

// Sets default values
AFP_Hero::AFP_Hero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFP_Hero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFP_Hero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFP_Hero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("HeroForward"), this, &AFP_Hero::HeroForward);
	PlayerInputComponent->BindAxis(TEXT("HeroLookUp"), this, &AFP_Hero::HeroLookUp);
	PlayerInputComponent->BindAxis(TEXT("HeroRight"), this, &AFP_Hero::HeroRight);
	PlayerInputComponent->BindAxis(TEXT("HeroLookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

}

void AFP_Hero::HeroForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}
void AFP_Hero::HeroRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}
void AFP_Hero::HeroLookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

