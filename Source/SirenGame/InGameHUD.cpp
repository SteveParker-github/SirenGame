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
