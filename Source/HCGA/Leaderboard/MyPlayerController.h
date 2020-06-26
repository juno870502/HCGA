// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "Leaderboards.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "steam/isteamuserstats.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLeaderboardRowData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Rank = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Nickname = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		int32 Time = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString Country = "";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FString ImgURL = "";
	
		UINT64 SteamID = 0;
};

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

UCLASS()
class HCGA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnLeaderboardReadComplete(bool bWasSuccessful);

	/** Internal. Reads the stats from the platform backend to sync online status with local */
	FOnlineLeaderboardReadPtr ReadObject;

	FDelegateHandle LeaderboardReadCompleteDelegateHandle;
	void ClearLeaderboardDelegate();

	/* WriteLeaderboard Variable */
	FHCGALeaderboardWrite WriteLeaderboardVariable;

	/** Updates leaderboard stats, force */
	UFUNCTION(BlueprintCallable)
	void WriteLeaderboard();

	UFUNCTION()
	void OnLeaderboardReadCompleteToWrite(bool bWasSuccessful);

	int32 TimeToWrite = 0;

	/** Read leaderboard */
	UFUNCTION(BlueprintCallable)
	void ReadLeaderboard();

	/* Fastest, Longest Leaderboard*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FLeaderboardRowData> BPDataArray;

	///** Read Tombstone leaderboard */
	//UFUNCTION(BlueprintImplementableEvent)
	//	void ReadTombstone();
	///** Updates Tombstone leaderboard stats, force */
	//UFUNCTION(BlueprintImplementableEvent)
	//	void WriteTombstone();

	/* TombStone Leaderboard*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<FTombStoneLeaderboard> BPTombArray;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWidget();

	/* Http Module*/
	FHttpModule* Http;

	/* Get User Location */
	void GetUserLocationCall();

	/*Assign this function to call when the GET request processes sucessfully*/
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);


	/* Using in steam*/
	SteamLeaderboard_t m_CurrentLeaderboard;
	int m_nLeaderboardEntries = 100; // How many entries do we have?
	LeaderboardEntry_t m_leaderboardEntries[100]; // The entries
	bool LeaderboardSelector;
	UFUNCTION(BlueprintCallable)
	void FindLeaderboard(const FString pchLeaderboardName);
	UFUNCTION(BlueprintCallable)
	bool UploadScore(int score);
	UFUNCTION(BlueprintCallable)
	bool DownloadScores();
	void OnFindLeaderboard(LeaderboardFindResult_t *pResult, bool bIOFailure);
	CCallResult<AMyPlayerController, LeaderboardFindResult_t> m_callResultFindLeaderboard;
	UFUNCTION(BlueprintImplementableEvent)
		void OnFindLeaderboardEvent();
	void OnUploadScore(LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	CCallResult < AMyPlayerController, LeaderboardScoreUploaded_t> m_callResultUploadScore;
	UFUNCTION(BlueprintImplementableEvent)
		void OnUploadLeaderboardEvent();
	void OnDownloadScore(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult < AMyPlayerController, LeaderboardScoresDownloaded_t> m_callResultDownloadScore;
	UFUNCTION(BlueprintImplementableEvent)
		void OnDownloadLeaderboardEvent();
};
