// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IngameGM.generated.h"

/**
 * 
 */
UCLASS()
class HCGA_API AIngameGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	AIngameGM();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int ClearItemNum;
};
