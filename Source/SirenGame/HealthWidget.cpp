// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"

UHealthWidget::UHealthWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UHealthWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UHealthWidget::UpdateBoatHealth(float HealthPercentage)
{
    if(ProgressBoatHealth)
    {
        ProgressBoatHealth->SetPercent(HealthPercentage);
        ProgressBoatHealth->SetFillColorAndOpacity(GetColour(HealthPercentage));

    }
}

void UHealthWidget::UpdateCrewHealth(float HealthPercentage)
{
    if(ProgressCrewHealth)
    {
        ProgressCrewHealth->SetPercent(HealthPercentage);
        ProgressCrewHealth->SetFillColorAndOpacity(GetColour(HealthPercentage));
    }
}

void UHealthWidget::UpdateArcherHealth(float HealthPercentage)
{
    if(ProgressArcherHealth)
    {
        ProgressArcherHealth->SetPercent(HealthPercentage);
        ProgressArcherHealth->SetFillColorAndOpacity(GetColour(HealthPercentage));
    }
}

FLinearColor UHealthWidget::GetColour(float Percentage)
{
    if (Percentage > 0.66f) return FLinearColor(0, 1, 0.1f, 1);
    if (Percentage < 0.33f) return FLinearColor(1, 0, 0.05f, 1);
    return FLinearColor(1, 0.9f, 0, 1);
}

