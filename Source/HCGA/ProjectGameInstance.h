// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HCGA_API UProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UProjectGameInstance();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int SelectCharacter = 0;

	UFUNCTION(BlueprintCallable)
	void ReadLeaderboard();

	UFUNCTION(BlueprintCallable)
	void WriteLeaderboard();

	void LoginLeaderboard();

	void OnLoginCompleteReadStats(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	void ReadStats();

	
	/** Handle to the registered LeaderboardReadComplete delegate */
	//FDelegateHandle LeaderboardReadCompleteDelegateHandle;

	/** Handle to the registered LoginComplete delegate */
	//FDelegateHandle OnLoginCompleteDelegateHandle;

	/** action bindings array */
	//TArray<TSharedPtr<FLeaderboardRow>> StatRows;

	/** Indicates that a stats read operation has been initiated */
	bool bReadingStats;
};
