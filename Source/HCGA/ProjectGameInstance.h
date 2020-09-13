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

	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int SelectCharacter = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int GameTimeScore = 0;
};
