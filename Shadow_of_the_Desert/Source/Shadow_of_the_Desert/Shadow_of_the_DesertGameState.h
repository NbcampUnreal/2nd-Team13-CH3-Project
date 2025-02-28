// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Shadow_of_the_DesertGameState.generated.h"

/**
 * 
 */
UCLASS()
class SHADOW_OF_THE_DESERT_API AShadow_of_the_DesertGameState : public AGameState
{
	GENERATED_BODY()
public:
	AShadow_of_the_DesertGameState();

	// 적을 죽인 횟수, 적의 총 수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 KillEnemyCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 AllEnemyCount;

	// 적 스폰 양
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 MinSpawnNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 MaxSpawnNum;

	// 게임이 일시정지 상태인지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
	bool bIsPaused;

	//일시정지창
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	//게임 종료창
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> EndMenuWidgetClass;

	// HUD 위젯 생성 및 관리
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	// 보스, 플레이어 사망 여부
	bool bIsBossDead;
	bool bIsPlayerDead;
	

	FTimerHandle GameTimerHandle;
	
	FTimerHandle EnemyTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	float LocalElapsedTime;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void LocalStartGame();

	// 게임 일시정지 처리
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalPauseGame();

	// 게임 재개 처리
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalResumeGame();

	// 게임 리셋 처리
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalResetGame();


	void SetHUDVisibility(bool bVisible);
	void SpawnBoss();
	void KillEnemy(int32 Score);	// 적을 죽었을때
	void EnemySpawn();
	void TimerUpdate();
	void UpdateHUD();
	void GameEnd(FString Result);	// 게임 종료

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bIsTimerRunning;
	UUserWidget* PauseMenuWidget;
	UUserWidget* EndMenuWidget;
	UUserWidget* HUDWidget;
};
