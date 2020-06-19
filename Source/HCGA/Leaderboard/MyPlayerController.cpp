// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "Leaderboards.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"
#include "OnlineSubsystemTypes.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Steamworks/Steamv146/sdk/public/steam/steam_api.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

AMyPlayerController::AMyPlayerController()
{
	Http = &FHttpModule::Get();
}

void AMyPlayerController::OnLeaderboardReadComplete(bool bWasSuccessful)
{
	if (ReadObject.IsValid() && ReadObject->ReadState == EOnlineAsyncTaskState::Done)
	{
		//bHasFetchedPlatformData = true;
		ClearLeaderboardDelegate();

		// We should only have one stat.
		if (bWasSuccessful)
		{
			for (size_t i = 0; i < ReadObject->Rows.Num(); i++)
			{
				FOnlineStatsRow& RowData = ReadObject->Rows[i];
				int32 Time;
				if (const FVariantData* TimeData = RowData.Columns.Find(LEADERBOARD_STAT_TIME))
				{
					FLeaderboardRowData BPData;
					TimeData->GetValue(Time);
					BPData.Rank = RowData.Rank;
					BPData.Nickname = RowData.NickName;
					BPData.Time = Time;

					uint64 int64ID = SteamUser()->GetSteamID().ConvertToUint64();
					
					GetUserLocationCall(int64ID);

					BPDataArray.Add(BPData);
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, FString::Printf(TEXT("UserID : %lld"), int64ID));
				}
			}
			UpdateWidget();
			BPDataArray.Empty();
		}
	}
}

void AMyPlayerController::ClearLeaderboardDelegate()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	if (OnlineSub)
	{
		IOnlineLeaderboardsPtr Leaderboards = OnlineSub->GetLeaderboardsInterface();
		if (Leaderboards.IsValid())
		{
			Leaderboards->ClearOnLeaderboardReadCompleteDelegate_Handle(LeaderboardReadCompleteDelegateHandle);
		}
	}
}

void AMyPlayerController::WriteLeaderboard()
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	if (SubSystem)
	{
		IOnlineIdentityPtr Identity = SubSystem->GetIdentityInterface();
		if (Identity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> UserIdPtr = Identity->GetUniquePlayerId(0);
			if (UserIdPtr.IsValid())
			{
				IOnlineLeaderboardsPtr Leaderboards = SubSystem->GetLeaderboardsInterface();
				if (Leaderboards.IsValid())
				{
					int TimeInteger;
					TimeInteger = UGameplayStatics::GetTimeSeconds(GetWorld()) * 100;
					//WriteLeaderboardVariable.FindStatByName(TEXT("Time"));
					WriteLeaderboardVariable.SetIntStat(TEXT("Time"), TimeInteger);
					
					Leaderboards->WriteLeaderboards(WriteLeaderboardVariable.LeaderboardNames[0], (*UserIdPtr), WriteLeaderboardVariable);
					Leaderboards->FlushLeaderboards(WriteLeaderboardVariable.LeaderboardNames[0]);

					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Black, FString::Printf(TEXT("TimeIntager : %d"), TimeInteger));
				}
			}
		}
	}
}

void AMyPlayerController::ReadLeaderboardToWrite()
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);

	if (SubSystem)
	{
		IOnlineIdentityPtr Identity = SubSystem->GetIdentityInterface();
		if (Identity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> UserIdPtr = Identity->GetUniquePlayerId(0);
			TSharedRef<const FUniqueNetId> UserIdRef = UserIdPtr.ToSharedRef();

			IOnlineLeaderboardsPtr Leaderboards = SubSystem->GetLeaderboardsInterface();

			if (Leaderboards.IsValid())
			{
				// We are about to read the stats. The delegate will set this to false once the read is complete.
				LeaderboardReadCompleteDelegateHandle = Leaderboards->OnLeaderboardReadCompleteDelegates.AddUObject(this, &AMyPlayerController::OnLeaderboardReadCompleteToWrite);

				ReadObject = MakeShareable(new FHCGALeaderboardRead());
				FOnlineLeaderboardReadRef ReadRef = ReadObject.ToSharedRef();

				// Read only one player, self.
				Leaderboards->ReadLeaderboardsAroundUser(UserIdRef, 0, ReadRef);

				TimeToWrite = UGameplayStatics::GetTimeSeconds(GetWorld()) * 100;
			}
		}
	}
}

void AMyPlayerController::OnLeaderboardReadCompleteToWrite(bool bWasSuccessful)
{

	FLeaderboardRowData CompareData;

	if (ReadObject.IsValid() && ReadObject->ReadState == EOnlineAsyncTaskState::Done)
	{
		//bHasFetchedPlatformData = true;
		ClearLeaderboardDelegate();
		
		// We should only have one stat.
		if (bWasSuccessful && ReadObject->Rows.Num() == 1)
		{
			FOnlineStatsRow& RowData = ReadObject->Rows[0];
			int32 Time;
			if (const FVariantData* TimeData = RowData.Columns.Find(LEADERBOARD_STAT_TIME))
			{
				TimeData->GetValue(Time);
				CompareData.Rank = RowData.Rank;
				CompareData.Nickname = RowData.NickName;
				CompareData.Time = Time;
				
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Black, FString::Printf(TEXT("TimeToWrite : %d"), Time));
			}
			
		}
	}
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	if (SubSystem)
	{
		IOnlineIdentityPtr Identity = SubSystem->GetIdentityInterface();
		if (Identity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> UserIdPtr = Identity->GetUniquePlayerId(0);
			if (UserIdPtr.IsValid())
			{
				IOnlineLeaderboardsPtr Leaderboards = SubSystem->GetLeaderboardsInterface();
				if (Leaderboards.IsValid())
				{
					if (CompareData.Time > TimeToWrite || CompareData.Time == 0)
					{
						WriteLeaderboardVariable.SetIntStat(TEXT("Time"), TimeToWrite);						

						Leaderboards->WriteLeaderboards(WriteLeaderboardVariable.LeaderboardNames[0], (*UserIdPtr), WriteLeaderboardVariable);
						Leaderboards->FlushLeaderboards(WriteLeaderboardVariable.LeaderboardNames[0]);

						//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Black, FString::Printf(TEXT("TimeIntager : %d"), TimeToWrite));
					}
				}
			}
		}
	}
}

void AMyPlayerController::ReadLeaderboard()
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);

	if (SubSystem)
	{
		IOnlineIdentityPtr Identity = SubSystem->GetIdentityInterface();
		if (Identity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> UserIdPtr = Identity->GetUniquePlayerId(0);
			TSharedRef<const FUniqueNetId> UserIdRef = UserIdPtr.ToSharedRef();

			IOnlineLeaderboardsPtr Leaderboards = SubSystem->GetLeaderboardsInterface();

			if (Leaderboards.IsValid())
			{
				// We are about to read the stats. The delegate will set this to false once the read is complete.
				LeaderboardReadCompleteDelegateHandle = Leaderboards->OnLeaderboardReadCompleteDelegates.AddUObject(this, &AMyPlayerController::OnLeaderboardReadComplete);

				ReadObject = MakeShareable(new FHCGALeaderboardRead());
				FOnlineLeaderboardReadRef ReadRef = ReadObject.ToSharedRef();

				Leaderboards->FreeStats(ReadRef.Get());

				Leaderboards->ReadLeaderboardsAroundRank(10, 10, ReadRef);
			}
		}
	}
}

void AMyPlayerController::GetUserLocationCall(uint64 &steamid)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AMyPlayerController::OnResponseReceived);
	//This is the url on which to process the request
	FString URL = FString::Printf(TEXT("https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=FEA5E223FDF8E24E86134FFF026F6F90&steamids=%lld"), steamid);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, URL);
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void AMyPlayerController::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//FString ResponseText = Response->GetContentAsString();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Start OnResponseReceived")));

	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	////Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Success Deserialize")));
		//Get the value of the json object by field name
		//JsonObject->GetStringField()
		TSharedPtr<FJsonObject> responseJson = JsonObject->GetObjectField("response");
		TArray<TSharedPtr<FJsonValue>> arr = responseJson->GetArrayField("players");
		for (size_t i = 0; i < arr.Num(); i++)
		{
			TSharedPtr<FJsonObject> obj = arr[i]->AsObject();
			FString CountryCode = obj->GetStringField("loccountrycode");
			//Output it to the engine
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, CountryCode);
		}
		
	}
}
