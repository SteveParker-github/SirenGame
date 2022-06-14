// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "HealthWidget.h"
#include "MinimapWidget.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SIRENS_CALL_API AInGameHUD : public AHUD
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

	UFUNCTION()
	void UpdateArcherHealth(float HealthPercentage);

	UFUNCTION()
	void UpdateStamina(float StaminaPercentage);

	UFUNCTION()
	void RemoveHUD();

	UFUNCTION()
	void AddHUD();

	UFUNCTION()
	void DeathHUD(int32 Zone);

	UFUNCTION()
	void AddDamage();

	UFUNCTION()
	void RemoveDamage();

	UFUNCTION()
	void AddTempDamage();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> HealthWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> MinimapWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> DeathScreenWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> DamageWidgetClass;

	UFUNCTION(BlueprintPure)
	int32 GetZoneLevel() const;


private:
	UHealthWidget* HealthWidget;
	UUserWidget* MinimapWidget;
	UUserWidget* DeathScreenWidget;
	UUserWidget* DamageWidget;

	int32 ZoneLevel;

	float DamageCooldown;
	bool bIsDamageWidget = false;
	bool bIsWidgetView = false;
};
