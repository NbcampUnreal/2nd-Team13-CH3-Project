// Fill out your copyright notice in the Description page of Project Settings.


#include "Shadow_of_the_DesertGameState.h"
#include "Shadow_of_the_DesertGameInstance.h"
#include "Player_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AShadow_of_the_DesertGameState::AShadow_of_the_DesertGameState()
{
	KillEnemyCount = 0;
	AllEnemyCount = 0;
	MinSpawnNum = 0;
	MaxSpawnNum = 0;
	bIsBossSpawned = false;
	bIsBossDead = false;
	bIsPlayerDead = false;
	bIsTimerRunning = false;
	bIsPaused = false;
}


void AShadow_of_the_DesertGameState::LocalStartGame()
{
	// ���� ���۽� �ʱ�ȭ
	if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SOTDInstance->TotalScore = 0;
		SOTDInstance->TotalDamageDealt = 0.0f;
		SOTDInstance->TotalDamageTaken = 0.0f;
	}

	// ���� Ÿ�̸� ����
	if (!bIsTimerRunning)
	{
		bIsTimerRunning = true;

		// 1�ʸ��� UpdateTimer �Լ� ����
		GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AShadow_of_the_DesertGameState::TimerUpdate, 1.f, true);
	}

	//ù����
	EnemySpawn();

	//�� ���� 10�ʸ��� ���� false���� true�� �ٲ��ָ��
	GetWorldTimerManager().SetTimer(
		EnemyTimerHandle,
		this,
		&AShadow_of_the_DesertGameState::EnemySpawn,
		10.0f,
		false);

	//UI ������Ʈ
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AShadow_of_the_DesertGameState::UpdateHUD,
		0.1f,
		false);
}

void AShadow_of_the_DesertGameState::LocalPauseGame()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetPause(true);
		GetWorldTimerManager().PauseTimer(GameTimerHandle);
		bIsPaused = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Game Paused"));
}

void AShadow_of_the_DesertGameState::LocalResumeGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Resumed"));
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetPause(false);
		GetWorldTimerManager().UnPauseTimer(GameTimerHandle);
		bIsPaused = false;
	}
	
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

void AShadow_of_the_DesertGameState::TimerUpdate()
{
	// ������ �Ͻ����� ���� �ƴ϶�� ��� �ð� 1�ʾ� ����
	if (!bIsPaused)
	{
		LocalElapsedTime += 1.0f;
		UE_LOG(LogTemp, Warning, TEXT("Elapsed Time: %.1f"), LocalElapsedTime);
	}
}

void AShadow_of_the_DesertGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		APlayer_Controller* Player_Controller = Cast<APlayer_Controller>(PlayerController);
		/*if (UUserWidget* HUDWidget = Player_Controller->GetHUDWidget())
		{
			if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("�ð�"))))
			{
				float ElapsedTime = GetWorldTimerManager().GetTimerElapsed(GameTimerHandle);

				TimeText->SetText(FText::FromString(FString::Printf(TEXT("Elapsed Time: %.1f"), ElapsedTime)));
			}
		}*/
	}
}

void AShadow_of_the_DesertGameState::GameEnd()
{
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	// ���� ���� ���� �ڵ� �޴�â ȣ��
}

void AShadow_of_the_DesertGameState::BeginPlay()
{
	Super::BeginPlay();

}



