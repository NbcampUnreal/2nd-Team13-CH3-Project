// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SOTDSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SHADOW_OF_THE_DESERT_API USOTDSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	float SavedMouseSensitivity = 1.0f;
};
