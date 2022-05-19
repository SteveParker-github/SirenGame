// Fill out your copyright notice in the Description page of Project Settings.


#include "FP_Character.h"

// Sets default values
AFP_Character::AFP_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFP_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("FPMoveForward"), this,  &AFP_Character::FPMoveForward);
	PlayerInputComponent->BindAxis(TEXT("FPLookUp"), this,  &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("FPMoveRight"), this,  &AFP_Character::FPMoveRight);
	PlayerInputComponent->BindAxis(TEXT("FPLookRight"), this,  &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("JUMP"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
}

void AFP_Character::FPMoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}
void AFP_Character::FPMoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}
