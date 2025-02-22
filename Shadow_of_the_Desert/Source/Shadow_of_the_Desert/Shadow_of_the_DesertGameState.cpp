// Fill out your copyright notice in the Description page of Project Settings.


#include "Shadow_of_the_DesertGameState.h"
#include "Shadow_of_the_DesertGameInstance.h"
#include "Blueprint/UserWidget.h"

AShadow_of_the_DesertGameState::AShadow_of_the_DesertGameState()
{
	KillEnemyCount = 0;
	AllEnemyCount = 0;
	MinSpawnNum = 0;
	MaxSpawnNum = 0;
	bIsBossSpawned = false;
	bIsBossDead = false;
	bIsPlayerDead = false;
}


void AShadow_of_the_DesertGameState::SpawnBoss()
{
	//보스 스폰 코드 작성
	bIsBossSpawned = true;
}

void AShadow_of_the_DesertGameState::KillEnemy()
{
	if (bIsBossDead && KillEnemyCount >= AllEnemyCount)
	{
		//보스와 모든 몬스터를 죽이면 게임 종료
		
		GameEnd();
	}
	else
	{
		if (AllEnemyCount <= 0 && KillEnemyCount >= AllEnemyCount)
		{
			return;
		}

		KillEnemyCount++;
		
		if (KillEnemyCount > 0 && KillEnemyCount >= AllEnemyCount)
		{
			EnemySpawn();
		}
	}
}

void AShadow_of_the_DesertGameState::EnemySpawn()
{
	if (!bIsBossSpawned)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime >= 60.0f)
		{
			SpawnBoss();
			GetWorldTimerManager().ClearTimer(EnemyTimerHandle);
		}

		//이 아래에 적 스폰 코드 작성
		int32 RandomSpawn = FMath::RandRange(MinSpawnNum, MaxSpawnNum);

		for (size_t i = 0; i < RandomSpawn; i++)
		{

		}
	}
}

void AShadow_of_the_DesertGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{

	}
	// 여기 UI HUD 작성 코드
}

void AShadow_of_the_DesertGameState::GameEnd()
{
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	// 여기 게임 종료 코드 메뉴창 호출
}

void AShadow_of_the_DesertGameState::BeginPlay()
{
	Super::BeginPlay();

	//첫스폰
	EnemySpawn();
	//그 이후 10초마다 스폰 false에서 true로 바꿔주면됨
	GetWorldTimerManager().SetTimer(
		EnemyTimerHandle,
		this,
		&AShadow_of_the_DesertGameState::EnemySpawn,
		10.0f,
		false);

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AShadow_of_the_DesertGameState::UpdateHUD,
		0.1f,
		false);
}
