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

UCLASS()
class HCGA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FLeaderboardRowData> BPDataArray;

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

	UFUNCTION(BlueprintCallable)
	void FindLeaderboard(const FString pchLeaderboardName);
	bool UploadScore(int score);
	bool DownloadScores();
	void OnFindLeaderboard(LeaderboardFindResult_t *pResult, bool bIOFailure);
	CCallResult<AMyPlayerController, LeaderboardFindResult_t> m_callResultFindLeaderboard;
	void OnUploadScore(LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	CCallResult < AMyPlayerController, LeaderboardScoreUploaded_t> m_callResultUploadScore;
	void OnDownloadScore(LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult < AMyPlayerController, LeaderboardScoresDownloaded_t> m_callResultDownloadScore;
};
