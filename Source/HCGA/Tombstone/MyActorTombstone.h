// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "steam/isteamuserstats.h"
#include "steam/steam_api.h"
#include "MyActorTombstone.generated.h"

USTRUCT(BlueprintType)
struct FTombStoneLeaderboard
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Nickname = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Date = 0;
};

struct Achievement_t
{
	int m_eAchievementID;
	const char *m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};

UCLASS()
class HCGA_API AMyActorTombstone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActorTombstone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FTombStoneLeaderboard> BPTombArray;
	/* Using in steam*/
	SteamLeaderboard_t m_CurrentLeaderboard;
	int m_nLeaderboardEntries = 20; // How many entries do we have?
	LeaderboardEntry_t m_leaderboardEntries[20]; // The entries
	/* Request Functions */
	UFUNCTION(BlueprintCallable)
	void FindLeaderboard(const FString pchLeaderboardName);
	UFUNCTION(BlueprintCallable)
	bool UploadScore(int score);
	UFUNCTION(BlueprintCallable)
	bool DownloadScores();
	/* Callback Functions */
	void OnFindLeaderboard(LeaderboardFindResult_t *pResult, bool bIOFailure);
	CCallResult<AMyActorTombstone, LeaderboardFindResult_t> m_callResultFindLeaderboard;
	void OnUploadScore(LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	CCallResult < AMyActorTombstone, LeaderboardScoreUploaded_t> m_callResultUploadScore;
	void OnDownloadScore(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult < AMyActorTombstone, LeaderboardScoresDownloaded_t> m_callResultDownloadScore;
	/* BP Implement Functions */
	UFUNCTION(BlueprintImplementableEvent)
	void OnFindLeaderboardEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void OnUploadLeaderboardEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDownloadLeaderboardEvent();

	/* Achievement Test*/
	int64 m_iAppID; // Current AppID
	Achievement_t *m_pAchievements; // Achievement Data
	int m_iNumAchievements; // Num of Achievement
	bool m_bInitialized; // Request Achievement Done?
	UFUNCTION(BlueprintCallable)
	void TestOnlyResetAhievements();
	bool RequestStats();
	void OnUserStatsReceived(UserStatsReceived_t * pCallback, bool bIOFailure);
	CCallResult < AMyActorTombstone, UserStatsReceived_t> m_CallbackUserStatsReceived;
};
