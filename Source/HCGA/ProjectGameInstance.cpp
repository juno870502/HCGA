// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "MoviePlayer/Public/MoviePlayer.h"

UProjectGameInstance::UProjectGameInstance()
{
	
}

void UProjectGameInstance::Init()
{
	Super::Init();
}

void UProjectGameInstance::LoginLeaderboard()
{
	//if (!OnLoginCompleteDelegateHandle.IsValid())
	//{
	//	//IOnlineSubsystem* const OnlineSub = Online::GetSubsystem(GetWorld());
	//	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	//	if (OnlineSub)
	//	{
	//		IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
	//		if (Identity.IsValid())
	//		{
	//			OnLoginCompleteDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateRaw(this, &UProjectGameInstance::OnLoginCompleteReadStats));
	//			Identity->Login(0, FOnlineAccountCredentials());
	//		}
	//	}
	//}
}


