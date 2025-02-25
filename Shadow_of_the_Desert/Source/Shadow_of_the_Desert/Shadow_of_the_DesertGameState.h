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

	// ���� ���� Ƚ��, ���� �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 KillEnemyCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 AllEnemyCount;

	// ������ �Ͻ����� �������� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
	bool bIsPaused;

	// �� ���� ��
	int32 MinSpawnNum;
	int32 MaxSpawnNum;

	// ����, �÷��̾� ��� ����
	bool bIsBossDead;
	bool bIsPlayerDead;
	

	FTimerHandle GameTimerHandle;
	
	FTimerHandle EnemyTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	float LocalElapsedTime;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void LocalStartGame();

	// ���� �Ͻ����� ó��
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalPauseGame();

	// ���� �簳 ó��
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
	// ���� ���� ����
	bool bIsBossSpawned;
	bool bIsTimerRunning;
};
