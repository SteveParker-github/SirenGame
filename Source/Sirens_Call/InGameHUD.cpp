// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"

AInGameHUD::AInGameHUD()
{
    
}

void AInGameHUD::DrawHUD()
{
    Super::DrawHUD();
}

void AInGameHUD::BeginPlay()
{
    Super::BeginPlay();

    if (HealthWidgetClass)
    {
        HealthWidget = CreateWidget<UHealthWidget>(GetWorld(), HealthWidgetClass);
        if (HealthWidget)
        {
            HealthWidget->AddToViewport();
        }
    }

    if (MinimapWidgetClass)
    {
        MinimapWidget = CreateWidget<UUserWidget>(GetWorld(), MinimapWidgetClass);
        if (MinimapWidget)
        {
            MinimapWidget->AddToViewport();
        }
    }

    RemoveHUD();
}

void AInGameHUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AInGameHUD::UpdateBoatHealth(float HealthPercentage)
{
    if (HealthWidget)
    {
        HealthWidget->UpdateBoatHealth(HealthPercentage);
    }
}

void AInGameHUD::UpdateCrewHealth(float HealthPercentage)
{
    if (HealthWidget)
    {
        HealthWidget->UpdateCrewHealth(HealthPercentage);
    }
}

void AInGameHUD::UpdateArcherHealth(float HealthPercentage)
{
    if (HealthWidget)
    {
        HealthWidget->UpdateArcherHealth(HealthPercentage);
    }
}

void AInGameHUD::UpdateStamina(float StaminaPercentage)
{
    if (HealthWidget)
    {
        HealthWidget->UpdateStamina(StaminaPercentage);
    }
}

void AInGameHUD::RemoveHUD()
{
    HealthWidget->RemoveFromViewport();
    // MinimapWidget->RemoveFromViewport();
}

void AInGameHUD::AddHUD()
{
    HealthWidget->AddToViewport();
    // MinimapWidget->AddToViewport();
}