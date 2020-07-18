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

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include <sstream>

#include "steam/steam_api.h"

AMyPlayerController::AMyPlayerController()
{
	Http = &FHttpModule::Get();
	// Initialize Steam
	bool bRet = SteamAPI_Init();
	// Create the SteamLeaderboards object if Steam was successfully initialized

	SteamAPI_RunCallbacks();
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// Shutdown Steam
	//SteamAPI_Shutdown();
}

void AMyPlayerController::OnLeaderboardReadComplete(bool bWasSuccessful)
{
	if (ReadObject.IsValid() && ReadObject->ReadState == EOnlineAsyncTaskState::Done)
	{
		//bHasFetchedPlatformData = true;
		ClearLeaderboardDelegate();

		if (bWasSuccessful)
		{
			TArray<UINT64> IDs;
			for (size_t i = 0; i < ReadObject->Rows.Num(); i++)
			{
				//TSharedPtr<FOnlineStatsRow> RowData = MakeShareable(new FOnlineStatsRow(ReadObject->Rows[i]));
				const FOnlineStatsRow* RowData = &ReadObject->Rows[i];
				// Find(LEADERBOARD_STAT_TIME)
				
				FLeaderboardRowData BPData;
				int32 Time;
				RowData->Columns.Find("Time")->GetValue(Time);

				////////////////////
				// Testing....
				for (auto Elem : ReadObject->Rows[i].Columns)
				{
					FString TempString = Elem.Key.ToString();
					//Elem.Value.ToString()
					//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TempString);
				}
				////////////////////

				BPData.Rank = RowData->Rank;
				BPData.Nickname = RowData->NickName;
				BPData.Time = Time;

				//PlayerID to SteamID
				uint64 int64ID = *(uint64*)RowData->PlayerId->GetBytes();

				BPData.SteamID = int64ID;

				IDs.Add(int64ID);

				BPDataArray.Add(BPData);

				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Silver, FString::Printf(TEXT("Time : %d"), Time));
				
			}

			// Request to Steam Web API
			//GetUserLocationCall(IDs);
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

void AMyPlayerController::WriteLeaderboard(const float& RealTime)
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);

	if (SubSystem)
	{
		IOnlineIdentityPtr Identity = SubSystem->GetIdentityInterface();
		if (Identity.IsValid())
		{
			TSharedPtr<const FUniqueNetId> UserIdPtr = Identity->GetUniquePlayerId(0);
			TSharedRef<const FUniqueNetId> UserIdRef = UserIdPtr.ToSharedRef();
			TArray<TSharedRef<const FUniqueNetId>> ArrayIdRef;
			ArrayIdRef.Add(UserIdRef);

			IOnlineLeaderboardsPtr Leaderboards = SubSystem->GetLeaderboardsInterface();

			if (Leaderboards.IsValid())
			{
				// We are about to read the stats. The delegate will set this to false once the read is complete.
				//LeaderboardReadCompleteDelegateHandle = Leaderboards->OnLeaderboardReadCompleteDelegates.AddUObject(this, &UBFL_Leaderboard::OnLeaderboardReadCompleteToWrite);
				LeaderboardReadCompleteDelegateHandle = Leaderboards->OnLeaderboardReadCompleteDelegates.AddUObject(this, &AMyPlayerController::OnLeaderboardReadCompleteToWrite);

				ReadObject = MakeShareable(new FHCGALeaderboardRead());
				FOnlineLeaderboardReadRef ReadRef = ReadObject.ToSharedRef();

				// Read only one player, self.
				Leaderboards->ReadLeaderboards(ArrayIdRef, ReadRef);

				TimeToWrite = RealTime * 100;
			}
		}
	}
}

void AMyPlayerController::OnLeaderboardReadCompleteToWrite(bool bWasSuccessful)
{
	// To Compare Leaderboard Data and Current Record
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
				// Not required
				//CompareData.Rank = RowData.Rank;
				//CompareData.Nickname = RowData.NickName;
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
						WriteLeaderboardVariable;
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
	BPDataArray.Empty();

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

				//Leaderboards->FreeStats(ReadRef.Get());
				//Leaderboards->ReadLeaderboardsAroundUser(UserIdRef, 20, ReadRef);
				Leaderboards->ReadLeaderboardsAroundRank(10, 10, ReadRef);
			}
		}
	}
}

void AMyPlayerController::GetUserLocationCall()
{
	// Make string for find player's profile
	FString IDs;
	for (size_t i = 0; i < BPDataArray.Num(); i++)
	{
		std::ostringstream oss;
		oss << BPDataArray[i].SteamID;
		FString TempID = oss.str().c_str();
		IDs.Append(TempID +",");
	}
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AMyPlayerController::OnResponseReceived);
	//This is the url on which to process the request
	FString URL = FString::Printf(TEXT("https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v2/?key=FEA5E223FDF8E24E86134FFF026F6F90&steamids="));
	URL.Append(IDs);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, URL);
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void AMyPlayerController::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//FString ResponseText = Response->GetContentAsString();
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Start OnResponseReceived")));

	//Create a pointer to hold the json serialized data
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	////Deserialize the json data given Reader and the actual object to deserialize
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Success Deserialize")));
		//Get the value of the json object by field name
		//JsonObject->GetStringField()
		TSharedPtr<FJsonObject> responseJson = JsonObject->GetObjectField("response");
		TArray<TSharedPtr<FJsonValue>> arr = responseJson->GetArrayField("players");
		for (size_t i = 0; i < arr.Num(); i++)
		{
			TSharedPtr<FJsonObject> obj = arr[i]->AsObject();
			FString CountryCode = obj->GetStringField("loccountrycode");
			FString ImgURL = obj->GetStringField("avatar");

			// Steam ID Validate
			INT64 SteamID;
			obj->TryGetNumberField("steamid", SteamID);
			UINT64 USteamID = SteamID;
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("Json ID : %lld"), USteamID));
			//Output it to the engine
			for (size_t j = 0; j < BPDataArray.Num(); j++)
			{
				if (BPDataArray[j].SteamID == USteamID)
				{
					BPDataArray[j].Country = CountryCode;
					BPDataArray[j].ImgURL = ImgURL;
					break;
				}
			}
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, CountryCode);
		}

	}
	UpdateWidget();
}

void AMyPlayerController::FindLeaderboard(const FString pchLeaderboardName)
{
	m_CurrentLeaderboard = NULL;

	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	if (SubSystem)
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard(TCHAR_TO_ANSI(*pchLeaderboardName));
		m_callResultFindLeaderboard.Set(hSteamAPICall, this, &AMyPlayerController::OnFindLeaderboard);
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("End of FindLeaderboard")));
	}
}

bool AMyPlayerController::UploadScore(int score)
{
	if (!m_CurrentLeaderboard)
		return false;

	SteamAPICall_t hSteamAPICall =
		SteamUserStats()->UploadLeaderboardScore(m_CurrentLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, score, NULL, 0);

	m_callResultUploadScore.Set(hSteamAPICall, this, &AMyPlayerController::OnUploadScore);

	return true;
}

bool AMyPlayerController::DownloadScores(const LeaderboardScope E_Scope)
{
	if (!m_CurrentLeaderboard)
		return false;

	// load the specified leaderboard data around the current user
	SteamAPICall_t hSteamAPICall = NULL;
	switch (E_Scope)
	{
	case LeaderboardScope::Personal:
		hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(m_CurrentLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, 0, 0);
		break;
	case LeaderboardScope::Global:
		hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(m_CurrentLeaderboard, k_ELeaderboardDataRequestGlobal, 1, 100);
		break;
	default:
		break;
	}
	
	m_callResultDownloadScore.Set(hSteamAPICall, this, &AMyPlayerController::OnDownloadScore);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("End of DownloadScores")));
	return true;
}

void AMyPlayerController::OnFindLeaderboard(LeaderboardFindResult_t * pResult, bool bIOFailure)
{
	// see if we encountered an error during the call
	if (!pResult->m_bLeaderboardFound || bIOFailure)
	{
		//OutputDebugString("Leaderboard could not be found\n");
		return;
	}

	m_CurrentLeaderboard = pResult->m_hSteamLeaderboard;
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("End of OnFindLeaderboard")));
	OnFindLeaderboardEvent();
}

void AMyPlayerController::OnUploadScore(LeaderboardScoreUploaded_t * pResult, bool bIOFailure)
{
	if (!pResult->m_bSuccess || bIOFailure)
	{
		
	}
	OnUploadLeaderboardEvent();
}

void AMyPlayerController::OnDownloadScore(LeaderboardScoresDownloaded_t * pResult, bool bIOFailure)
{
	BPDataArray.Empty();
	m_nLeaderboardEntries = 100;	// init number of entries
	//BPTombArray.Empty();
	if (!bIOFailure)
	{
		//m_nLeaderboardEntries = min(pCallback->m_cEntryCount, 10);
		if (pResult->m_cEntryCount < m_nLeaderboardEntries)
		{
			m_nLeaderboardEntries = pResult->m_cEntryCount;
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("EntryCount : %d"), m_nLeaderboardEntries));
		}
		
		
		for (int index = 0; index < m_nLeaderboardEntries; index++)
		{
			SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, index, &m_leaderboardEntries[index], NULL, 0);
			FLeaderboardRowData Data;
			Data.SteamID = m_leaderboardEntries[index].m_steamIDUser.ConvertToUint64();
			Data.Nickname = SteamFriends()->GetFriendPersonaName(m_leaderboardEntries[index].m_steamIDUser);
			Data.Rank = m_leaderboardEntries[index].m_nGlobalRank;
			Data.Time = m_leaderboardEntries[index].m_nScore;
			BPDataArray.Add(Data);
		}
		GetUserLocationCall();
		
	}
	
	OnDownloadLeaderboardEvent();
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("End of OnDownloadScore")));
}
