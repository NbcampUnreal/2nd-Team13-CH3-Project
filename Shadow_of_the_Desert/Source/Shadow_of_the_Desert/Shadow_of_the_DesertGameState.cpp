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
	// 게임 시작시 초기화
	if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SOTDInstance->TotalScore = 0;
		SOTDInstance->TotalDamageDealt = 0.0f;
		SOTDInstance->TotalDamageTaken = 0.0f;
	}

	// 게임 타이머 시작
	if (!bIsTimerRunning)
	{
		bIsTimerRunning = true;

		// 1초마다 UpdateTimer 함수 실행
		GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AShadow_of_the_DesertGameState::TimerUpdate, 1.f, true);
	}

	//첫스폰
	EnemySpawn();

	//그 이후 10초마다 스폰 false에서 true로 바꿔주면됨
	GetWorldTimerManager().SetTimer(
		EnemyTimerHandle,
		this,
		&AShadow_of_the_DesertGameState::EnemySpawn,
		10.0f,
		false);

	//UI 업데이트
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

void AShadow_of_the_DesertGameState::TimerUpdate()
{
	// 게임이 일시정지 중이 아니라면 경과 시간 1초씩 누적
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
			if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("시간"))))
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
	// 여기 게임 종료 코드 메뉴창 호출
}

void AShadow_of_the_DesertGameState::BeginPlay()
{
	Super::BeginPlay();

}



