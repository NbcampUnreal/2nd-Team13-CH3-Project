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
	//���� ���� �ڵ� �ۼ�
	bIsBossSpawned = true;
}

void AShadow_of_the_DesertGameState::KillEnemy()
{
	if (bIsBossDead && KillEnemyCount >= AllEnemyCount)
	{
		//������ ��� ���͸� ���̸� ���� ����
		
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

		//�� �Ʒ��� �� ���� �ڵ� �ۼ�
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
	// ���� UI HUD �ۼ� �ڵ�
}

void AShadow_of_the_DesertGameState::GameEnd()
{
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	// ���� ���� ���� �ڵ� �޴�â ȣ��
}

void AShadow_of_the_DesertGameState::BeginPlay()
{
	Super::BeginPlay();

	//ù����
	EnemySpawn();
	//�� ���� 10�ʸ��� ���� false���� true�� �ٲ��ָ��
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
