// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Shadow_of_the_DesertGameMode.generated.h"

UCLASS(minimalapi)
class AShadow_of_the_DesertGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AShadow_of_the_DesertGameMode();

protected:
	virtual void BeginPlay() override;

	// 게임시작 메인 메뉴
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
};



