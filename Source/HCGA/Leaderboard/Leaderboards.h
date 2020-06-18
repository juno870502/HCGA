// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Interfaces/OnlineLeaderboardInterface.h"

// these are normally exported from platform-specific tools
#define LEADERBOARD_STAT_TIME				"Time"
#define LEADERBOARD_STAT_DATE				"Date"
#define LEADERBOARD_STAT_DEATHS				"Deaths"
#define LEADERBOARD_STAT_MATCHESPLAYED		"MatchesPlayed"

/**
 *	'AllTime' leaderboard read object
 */
class FHCGALeaderboardRead : public FOnlineLeaderboardRead
{
public:

	FHCGALeaderboardRead()
	{
		// Default properties
		LeaderboardName = FName(TEXT("Time"));
		SortedColumn = LEADERBOARD_STAT_TIME;

		// Define default columns
		new (ColumnMetadata) FColumnMetaData(LEADERBOARD_STAT_TIME, EOnlineKeyValuePairDataType::Int32);
		//new (ColumnMetadata) FColumnMetaData(LEADERBOARD_STAT_DATE, EOnlineKeyValuePairDataType::Int32);
		//new (ColumnMetadata) FColumnMetaData(LEADERBOARD_STAT_DEATHS, EOnlineKeyValuePairDataType::Int32);
		//new (ColumnMetadata) FColumnMetaData(LEADERBOARD_STAT_MATCHESPLAYED, EOnlineKeyValuePairDataType::Int32);
	}
};

/**	
 *	'AllTime' leaderboard write object
 */
class FHCGALeaderboardWrite : public FOnlineLeaderboardWrite
{
public:

	FHCGALeaderboardWrite()
	{
		// Default properties
		new (LeaderboardNames) FName(TEXT("Time"));
		RatedStat = LEADERBOARD_STAT_TIME;
		DisplayFormat = ELeaderboardFormat::Milliseconds;
		SortMethod = ELeaderboardSort::Ascending;
		UpdateMethod = ELeaderboardUpdateMethod::KeepBest;
	}
};

