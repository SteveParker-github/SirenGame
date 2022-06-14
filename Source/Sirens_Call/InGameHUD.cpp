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

    DamageWidget = CreateWidget<UUserWidget>(GetWorld(), DamageWidgetClass);
    DamageWidget->AddToViewport();
    DamageWidget->SetVisibility(ESlateVisibility::Hidden);
    RemoveHUD();
}

void AInGameHUD::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsDamageWidget)
    {
        if (DamageCooldown < 0)
        {
            RemoveDamage();
            bIsDamageWidget = false;
        }
        DamageCooldown -= DeltaSeconds;
    }
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
}

void AInGameHUD::AddHUD()
{
    HealthWidget->AddToViewport();
}

void AInGameHUD::DeathHUD(int32 Zone)
{
    ZoneLevel = Zone;
    MinimapWidget->RemoveFromViewport();
    DeathScreenWidget = CreateWidget<UUserWidget>(GetWorld(), DeathScreenWidgetClass);
    if (DeathScreenWidget)
    {
        DeathScreenWidget->AddToViewport();
    }
}

void AInGameHUD::AddDamage()
{
    if (bIsWidgetView)
    {
        return;
    }

    DamageWidget->SetVisibility(ESlateVisibility::Visible);
    bIsWidgetView = true;
}

void AInGameHUD::RemoveDamage()
{
    if (!bIsWidgetView)
    {
        return;
    }

    DamageWidget->SetVisibility(ESlateVisibility::Hidden);
    bIsWidgetView = false;
}

void AInGameHUD::AddTempDamage()
{
    if (bIsWidgetView)
    {
        return;
    }

    AddDamage();
    DamageCooldown = 1.0f;
    bIsDamageWidget = true;
}

int32 AInGameHUD::GetZoneLevel() const
{
    return ZoneLevel;
}

