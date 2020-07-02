// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorTombstone.h"
#include "Kismet/KismetMathLibrary.h"
#include "OnlineSubsystem.h"

// Sets default values
AMyActorTombstone::AMyActorTombstone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActorTombstone::BeginPlay()
{
	Super::BeginPlay();
	
	//m_iAppID = SteamUtils()->GetAppID();
	//m_pAchievements = NULL;
	//m_iNumAchievements = 1;
	RequestStats();
}

// Called every frame
void AMyActorTombstone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActorTombstone::FindLeaderboard(const FString pchLeaderboardName)
{
	m_CurrentLeaderboard = NULL;

	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	if (SubSystem)
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard(TCHAR_TO_ANSI(*pchLeaderboardName));
		m_callResultFindLeaderboard.Set(hSteamAPICall, this, &AMyActorTombstone::OnFindLeaderboard);
	}
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("End of FindLeaderboard")));
}

bool AMyActorTombstone::UploadScore(int score)
{
	if (!m_CurrentLeaderboard)
		return false;

	SteamAPICall_t hSteamAPICall =
		SteamUserStats()->UploadLeaderboardScore(m_CurrentLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, score, NULL, 0);

	m_callResultUploadScore.Set(hSteamAPICall, this, &AMyActorTombstone::OnUploadScore);

	return true;
}

bool AMyActorTombstone::DownloadScores()
{
	if (!m_CurrentLeaderboard)
		return false;

	// load the specified leaderboard data around the current user
	SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(
		m_CurrentLeaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -9, 10);
	m_callResultDownloadScore.Set(hSteamAPICall, this, &AMyActorTombstone::OnDownloadScore);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("End of DownloadScores")));
	return true;
}

void AMyActorTombstone::OnFindLeaderboard(LeaderboardFindResult_t * pResult, bool bIOFailure)
{
	// see if we encountered an error during the call
	if (!pResult->m_bLeaderboardFound || bIOFailure)
	{
		//OutputDebugString("Leaderboard could not be found\n");
		return;
	}

	m_CurrentLeaderboard = pResult->m_hSteamLeaderboard;
	// Call BP event
	OnFindLeaderboardEvent();
}

void AMyActorTombstone::OnUploadScore(LeaderboardScoreUploaded_t * pResult, bool bIOFailure)
{
	if (!pResult->m_bSuccess || bIOFailure)
	{
		//OutputDebugString("Score could not be uploaded to Steam\n");
	}
	// Call BP event
	OnUploadLeaderboardEvent();
}

void AMyActorTombstone::OnDownloadScore(LeaderboardScoresDownloaded_t * pResult, bool bIOFailure)
{
	BPTombArray.Empty();
	if (!bIOFailure)
	{
		m_nLeaderboardEntries = UKismetMathLibrary::Min(pResult->m_cEntryCount, 10);

		for (int index = 0; index < m_nLeaderboardEntries; index++)
		{
			SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, index, &m_leaderboardEntries[index], NULL, 0);
			FTombStoneLeaderboard Data;
			Data.Date = m_leaderboardEntries[index].m_nScore;
			Data.Nickname = SteamFriends()->GetFriendPersonaName(m_leaderboardEntries[index].m_steamIDUser);
			BPTombArray.Add(Data);
		}
	}
	// Call BP event
	OnDownloadLeaderboardEvent();
}

void AMyActorTombstone::TestOnlyResetAhievements()
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	if (SubSystem)
	{
		SteamUserStats()->ClearAchievement("ACH_G_A");
	}
}

bool AMyActorTombstone::RequestStats()
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
	if (SubSystem)
	{
		SteamAPICall_t hSteamAPICall = SteamUserStats()->RequestUserStats(SteamUser()->GetSteamID());
		m_CallbackUserStatsReceived.Set(hSteamAPICall, this, &AMyActorTombstone::OnUserStatsReceived);
		return true;
	}
	return false;
}

void AMyActorTombstone::OnUserStatsReceived(UserStatsReceived_t *pCallback, bool bIOFailure)
{
	// Other game callback ignored.
	if (m_iAppID == pCallback->m_nGameID)
	{
		if (k_EResultOK == pCallback->m_eResult)
		{
			//OutputDebugString("Received stats and achievements from Steam\n");
			m_bInitialized = true;

			// Get achievement.
			for (int iAch = 0; iAch < m_iNumAchievements; ++iAch)
			{
				Achievement_t &ach = m_pAchievements[iAch];

				SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);
				_snprintf(ach.m_rgchName, sizeof(ach.m_rgchName), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"name"));
				_snprintf(ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s",
					SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID,
						"desc"));
			}
		}
		else
		{
			char buffer[128];
			_snprintf(buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult);
			//OutputDebugString(buffer);
		}
	}
}

