// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UIntroWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartButton")));

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UIntroWidget::StartGame);
	}
}

void UIntroWidget::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"), true);
}
