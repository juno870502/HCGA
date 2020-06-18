// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class HCGA_API USelectWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UButton* HemekoButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UButton* ChoChoButton;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		class UButton* GameStartButton;

	UFUNCTION()
		void SelectHemeko();

	UFUNCTION()
		void SelectChoCho();

	UFUNCTION()
		void GameSTart();
};
