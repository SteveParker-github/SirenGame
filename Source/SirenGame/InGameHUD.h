// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "HealthWidget.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SIRENGAME_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AInGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void UpdateBoatHealth(float HealthPercentage);

	UFUNCTION()
	void UpdateCrewHealth(float HealthPercentage);

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> HealthWidgetClass;

private:
	UHealthWidget* HealthWidget;
};