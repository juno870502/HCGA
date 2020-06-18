// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
////////////////////////////////////////////
#include "HCGA/ProjectGameInstance.h"
/////////////////////////////////////
void USelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HemekoButton = Cast<UButton>(GetWidgetFromName(TEXT("Hemeko")));
	ChoChoButton = Cast<UButton>(GetWidgetFromName(TEXT("ChoCho")));
	GameStartButton = Cast<UButton>(GetWidgetFromName(TEXT("GameStart")));

	if (HemekoButton)
	{
		HemekoButton->OnClicked.AddDynamic(this, &USelectWidget::SelectHemeko);
	}

	if (ChoChoButton)
	{
		ChoChoButton->OnClicked.AddDynamic(this, &USelectWidget::SelectChoCho);
	}

	if (GameStartButton)
	{
		GameStartButton->OnClicked.AddDynamic(this, &USelectWidget::GameSTart);
	}
}

void USelectWidget::SelectHemeko()
{
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("RoomTest"), true);
	UProjectGameInstance* GI = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->SelectCharacter = 1;
}

void USelectWidget::SelectChoCho()
{
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("RoomTest"), true);
	UProjectGameInstance* GI = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->SelectCharacter = 2;
}

void USelectWidget::GameSTart()
{
	UProjectGameInstance* GI = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI->SelectCharacter > 0)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("RoomTestLight"), true);
	}
	
}
