// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SlateBasics.h"
#include "Interfaces/OnlineLeaderboardInterface.h"

/** leaderboard row display information */
struct FLeaderboardRow
{
	/** player rank*/
	FString Rank;

	/** player name */
	FString PlayerName;

	/** player total kills to display */
	FString Kills;

	/** player total deaths to display */
	FString Deaths;

	/** player Cleat Time to display */
	FString Time;

	/** Unique Id for the player at this rank */
	const TSharedPtr<const FUniqueNetId> PlayerId;

	/** Default Constructor */
	FLeaderboardRow(const FOnlineStatsRow& Row);
};

class HCGA_API SDTLeaderboard : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDTLeaderboard)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	int32 Time = 100;

	FOnlineLeaderboardReadPtr ReadObject;

	/** Delegate called when a leaderboard has been successfully read */
	FOnLeaderboardReadCompleteDelegate LeaderboardReadCompleteDelegate;
	/** Handle to the registered LeaderboardReadComplete delegate */
	FDelegateHandle LeaderboardReadCompleteDelegateHandle;

	void OnLeaderboardReadComplete(bool bWasSuccessful);

	UFUNCTION(BlueprintCallable)
	void ReadLeaderboard();

	UFUNCTION(BlueprintCallable)
	void WriteLeaderboard();

	/** action bindings array */
	TArray< TSharedPtr<FLeaderboardRow> > StatRows;

	/** Indicates that a stats read operation has been initiated */
	bool bReadingStats;

	/** Removes the bound LeaderboardReadCompleteDelegate if possible*/
	void ClearOnLeaderboardReadCompleteDelegate();

	/** Returns true if a leaderboard read request is in progress or scheduled */
	bool IsLeaderboardReadInProgress();
};
