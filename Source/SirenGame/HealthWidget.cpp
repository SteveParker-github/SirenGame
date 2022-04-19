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
    }
}

void UHealthWidget::UpdateCrewHealth(float HealthPercentage)
{
    if(ProgressCrewHealth)
    {
        ProgressCrewHealth->SetPercent(HealthPercentage);
    }
}