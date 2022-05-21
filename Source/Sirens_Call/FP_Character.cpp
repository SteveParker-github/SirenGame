// Fill out your copyright notice in the Description page of Project Settings.

#include "FP_Character.h"
#include "Kismet/GameplayStatics.h"
#include "BoatPawn.h"

// Sets default values
AFP_Character::AFP_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
	TriggerZone->SetupAttachment(this->GetRootComponent());

	TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AFP_Character::OnOverlapBegin);

	bIsCurrentlyPossessed = true;
	bCanSwap = false;
}

// Called when the game starts or when spawned
void AFP_Character::BeginPlay()
{
	Super::BeginPlay();

	BoatPawn = Cast<ABoatPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoatPawn::StaticClass()));
}

// Called every frame
void AFP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFP_Character::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("FPMoveForward"), this, &AFP_Character::FPMoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFP_Character::FPMoveForward);
	PlayerInputComponent->BindAxis(TEXT("FPLookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("FPMoveRight"), this, &AFP_Character::FPMoveRight);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFP_Character::FPMoveRight);
	PlayerInputComponent->BindAxis(TEXT("FPLookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("JUMP"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Activate"), EInputEvent::IE_Pressed, this, &AFP_Character::SwapCharacter);
}

void AFP_Character::OnOverlapBegin(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Player: Trigger something"));

	if (bIsCurrentlyPossessed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player: activate message to swap"));
		bCanSwap = true;
	}
}

void AFP_Character::OnOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
	if (bIsCurrentlyPossessed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player: de-activate message to swap"));
		bCanSwap = false;
	}
}

void AFP_Character::FPMoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}
void AFP_Character::FPMoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AFP_Character::SwapCharacter()
{
	if (bCanSwap)
	{
		SavedController = GetController();
		SavedController->UnPossess();
		SavedController->Possess(Cast<APawn>(BoatPawn));
		bIsCurrentlyPossessed = false;
		BoatPawn->bIsCurrentlyPossessed = true;
		BoatPawn->AddHUD();
		BoatPawn->bCanSwap = false;
		bCanSwap = false;
	}
}
