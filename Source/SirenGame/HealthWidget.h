// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIRENGAME_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	UHealthWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UProgressBar* ProgressBoatHealth;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, meta = (BindWidget))
	class UProgressBar* ProgressCrewHealth;

	void UpdateBoatHealth(float HealthPercentage);

	void UpdateCrewHealth(float HealthPercentage);


};
