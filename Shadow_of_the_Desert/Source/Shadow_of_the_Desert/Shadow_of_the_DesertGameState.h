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

	// 게임이 일시정지 상태인지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
	bool bIsPaused;

	// 적 스폰 양
	int32 MinSpawnNum;
	int32 MaxSpawnNum;

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

	void SpawnBoss();
	void KillEnemy();
	void EnemySpawn();
	void TimerUpdate();
	void UpdateHUD();
	void GameEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 보스 스폰 여부
	bool bIsBossSpawned;
	bool bIsTimerRunning;
};
