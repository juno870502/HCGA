// Fill out your copyright notice in the Description page of Project Settings.


#include "IngameGM.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

//////////////////////////////////////
#include "HCGA/ProjectGameInstance.h"
#include "HCGA/Ingame/IngameCharacter.h"
//////////////////////////////////////

AIngameGM::AIngameGM()
{
	

	//if (GI->SelectCharacter == 2)
	//{

	//}
}


void AIngameGM::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//UProjectGameInstance* GI = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//if (GI->SelectCharacter == 1)
	//{
	//	static ConstructorHelpers::FClassFinder<APawn> Pawn_Class(TEXT("Blueprint'/Game/DTChar/BP/BP_DTCharPA_HMK.BP_DTCharPA_HMK_C'"));
	//	//static ConstructorHelpers::FObjectFinder<AIngameCharacter> Pawn_Class(TEXT("Blueprint'/Game/DTChar/BP/BP_DTCharPA.BP_DTCharPA_C'"));
	//	if (Pawn_Class.Succeeded())
	//	{
	//		DefaultPawnClass = Pawn_Class.Class;
	//	}
	//}
	//else if (GI->SelectCharacter == 2)
	//{
	//	static ConstructorHelpers::FClassFinder<AIngameCharacter> Pawn_Class(TEXT("Blueprint'/Game/DTChar/BP/BP_DTCharPA_CC.BP_DTCharPA_CC_C''"));
	//	//static ConstructorHelpers::FObjectFinder<AIngameCharacter> Pawn_Class(TEXT("Blueprint'/Game/DTChar/BP/BP_DTCharPA.BP_DTCharPA_C'"));
	//	if (Pawn_Class.Succeeded())
	//	{
	//		DefaultPawnClass = Pawn_Class.Class;
	//	}
	//}
}

void AIngameGM::BeginPlay()
{
	Super::BeginPlay();
	//UProjectGameInstance* GI = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	//ClearItemNum = 0;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		UAnimMontage* DeadAnimation;*/

}
