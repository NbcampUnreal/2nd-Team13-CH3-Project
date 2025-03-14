// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Shadow_of_the_DesertGameMode.generated.h"

UCLASS(minimalapi)
class AShadow_of_the_DesertGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AShadow_of_the_DesertGameMode();

	// 메인 메뉴 호출
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void ShowMainMenu();

	void CloseMainMenu();

	void RestartGame(); // 플레이어 리스폰 함수
protected:
	virtual void BeginPlay() override;

	// 게임시작 메인 메뉴
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:

	UUserWidget* MainMenuWidget;
};



