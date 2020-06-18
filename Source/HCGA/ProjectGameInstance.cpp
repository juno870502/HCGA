// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

UProjectGameInstance::UProjectGameInstance()
{
	
}

void UProjectGameInstance::ReadLeaderboard()
{
	//DTLB.ReadLeaderboard();
}

void UProjectGameInstance::WriteLeaderboard()
{
	//DTLB.WriteLeaderboard();
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

void UProjectGameInstance::OnLoginCompleteReadStats(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	//Online::GetIdentityInterface(GetWorld())->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, OnLoginCompleteDelegateHandle);
	//IOnlineSubsystem::Get(STEAM_SUBSYSTEM)->GetIdentityInterface()->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, OnLoginCompleteDelegateHandle);
	//if (bWasSuccessful)
	//{
	//	ReadStats();
	//}
}

void UProjectGameInstance::ReadStats()
{
}
