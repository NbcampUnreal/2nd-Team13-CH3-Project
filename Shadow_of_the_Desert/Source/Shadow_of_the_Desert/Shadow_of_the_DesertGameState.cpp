// Fill out your copyright notice in the Description page of Project Settings.


#include "Shadow_of_the_DesertGameState.h"
#include "Shadow_of_the_DesertGameMode.h"
#include "Shadow_of_the_DesertGameInstance.h"
#include "Shadow_of_the_DesertCharacter.h"
#include "EnemySpawner.h"
#include "Player_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AShadow_of_the_DesertGameState::AShadow_of_the_DesertGameState()
{
	KillEnemyCount = 0;
	AllEnemyCount = 0;
	MinSpawnNum = 10;
	MaxSpawnNum = 15;
	bIsBossDead = false;
	bIsPlayerDead = false;
	bIsTimerRunning = false;
	bIsPaused = false;
}


void AShadow_of_the_DesertGameState::LocalStartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Started"));

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

	// 게임 시작시 HUD 위젯 생성 및 추가
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	//UI 업데이트
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AShadow_of_the_DesertGameState::UpdateHUD,
		0.1f,
		false);

	// 메인메뉴 닫기
	if (AShadow_of_the_DesertGameMode* GameMode = Cast<AShadow_of_the_DesertGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->CloseMainMenu();
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
}

void AShadow_of_the_DesertGameState::LocalPauseGame()
{
	// Pause Menu 생성 및 화면에 추가
	if (PauseMenuWidgetClass && PauseMenuWidget == nullptr)
	{
		PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
		if (PauseMenuWidget)
		{
			// HUD 숨기기
			SetHUDVisibility(false);

			PauseMenuWidget->AddToViewport();

			// UI 모드로 변경
			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				GetWorldTimerManager().PauseTimer(GameTimerHandle);
				bIsPaused = true;

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->bShowMouseCursor = true;
				PC->SetInputMode(InputMode);
				PC->SetPause(true);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Game Paused"));
}

void AShadow_of_the_DesertGameState::LocalResumeGame()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetPause(false);
		GetWorldTimerManager().UnPauseTimer(GameTimerHandle);
		bIsPaused = false;

		// 게임 모드로 변경
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}

	// Pause Menu 제거
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromViewport();
		PauseMenuWidget = nullptr;

		// HUD 다시 표시
		SetHUDVisibility(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Game Resumed"));
}

void AShadow_of_the_DesertGameState::SetHUDVisibility(bool bVisible)
{
	if (HUDWidget)
	{
		// HUD의 가시성을 조절
		ESlateVisibility VisibilityState = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		HUDWidget->SetVisibility(VisibilityState);
	}
}

void AShadow_of_the_DesertGameState::SpawnBoss()
{
	//보스 스폰 코드 작성
	bIsBossDead = true;
}

void AShadow_of_the_DesertGameState::KillEnemy(int32 Score)
{
	if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SOTDInstance->TotalScore += Score;
	}

	KillEnemyCount++;

	//게임 종료 조건 모든 몬스터랑 보스를 죽이면
	if (bIsBossDead && KillEnemyCount >= AllEnemyCount)
	{
		GameEnd("Clear");
	}
	else
	{
		if (KillEnemyCount > 0 && KillEnemyCount >= AllEnemyCount)
		{
			EnemySpawn();
		}
	}
}

void AShadow_of_the_DesertGameState::EnemySpawn()
{
	// 보스 출현시간
	if (LocalElapsedTime >= 60.0f)
	{
		// 보스 소환 후 몬스터 스폰 멈추기(지금은 끝나는거 구분만)
		SpawnBoss();
		GetWorldTimerManager().ClearTimer(EnemyTimerHandle);

		return;
	}

	// Min~Max 랜덤 숫자만큼 스폰시키기
	int32 RandomSpawn = FMath::RandRange(MinSpawnNum, MaxSpawnNum);
	AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(this, AEnemySpawner::StaticClass()));

	// 스포너 배치되어있는지 확인(나중에 빼도 되는 코드?)
	if (!EnemySpawner)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawner not found"));
		return;
	}
	// 몬스터 스폰
	for (size_t i = 0; i < RandomSpawn; i++)
	{
		EnemySpawner->EnemySpawn();
		AllEnemyCount++;
	}
}

void AShadow_of_the_DesertGameState::TimerUpdate()
{
	// 게임이 일시정지 중이 아니라면 경과 시간 1초씩 누적
	if (!bIsPaused)
	{
		LocalElapsedTime += 1.0f;
		UE_LOG(LogTemp, Warning, TEXT("Elapsed Time: %.1f"), LocalElapsedTime);

		// 분마다 난이도 올라가게끔?
		int CurrentMinutes = FMath::FloorToInt(LocalElapsedTime / 60.0f);
		static int PreviousMinutes = 0;

		if (CurrentMinutes > PreviousMinutes)
		{
			MinSpawnNum += 5;
			MaxSpawnNum += 5;

			PreviousMinutes = CurrentMinutes;
			UE_LOG(LogTemp, Warning, TEXT("%d Minute"), CurrentMinutes);
		}
	}
}

void AShadow_of_the_DesertGameState::UpdateHUD()
{
	//if (HUDWidget)
	//{
	//	// 점수 표시
	//	UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText")));
	//	if (ScoreText)
	//	{
	//		if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
	//		{
	//			int32 Score = SOTDInstance->TotalScore;
	//			ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
	//		}
	//	}

	//	// 진행 시간 표시
	//	UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText")));
	//	if (TimeText)
	//	{
	//		int32 Minutes = static_cast<int32>(LocalElapsedTime) / 60;  // 분 계산
	//		int32 Seconds = static_cast<int32>(LocalElapsedTime) % 60;  // 초 계산

	//		TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %02d:%02d"), Minutes, Seconds)));
	//	}

	//	// 현재 플레이어 컨트롤러를 통해 조종 중인 캐릭터 가져오기
	//	ACharacter* PlayerCharacter = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//	if (PlayerCharacter)
	//	{
	//		AShadow_of_the_DesertCharacter* MyCharacter = Cast<AShadow_of_the_DesertCharacter>(PlayerCharacter);

	//		if (MyCharacter)
	//		{
	//			int32 PlayerHP = MyCharacter->Health;
	//			int32 MaxHP = MyCharacter->MaxHealth;

	//			// 체력 정보를 HUD에 반영
	//			UTextBlock* HealthText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("HealthText")));
	//			if (HealthText)
	//			{
	//				HealthText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), PlayerHP, MaxHP)));
	//			}

	//			UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar")));
	//			if (HealthBar)
	//			{
	//				float HPPercent = static_cast<float>(PlayerHP) / MaxHP;
	//				HealthBar->SetPercent(HPPercent);
	//			}
	//		}
	//	}
	//}
}

void AShadow_of_the_DesertGameState::LocalResetGame()
{
	// 현재 실행 중인 맵 이름 가져오기
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // 필요할 경우 접두사 제거

	// 현재 맵 다시 로드
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}

void AShadow_of_the_DesertGameState::GameEnd(FString Result)
{
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);

	// 게임 종료 UI 생성 및 표시
	if (EndMenuWidgetClass && EndMenuWidget == nullptr)
	{
		EndMenuWidget = CreateWidget<UUserWidget>(GetWorld(), EndMenuWidgetClass);
		if (EndMenuWidget)
		{
			EndMenuWidget->AddToViewport();

			// UI 모드로 변경 (마우스 활성화)
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(EndMenuWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(InputMode);
			}

			// 점수판같은건 여기다가

			// 승리 패배시 바뀌는값은 여기다가
			if (Result == "Clear")
			{
				UWidget* DefeatLogo = EndMenuWidget->GetWidgetFromName(TEXT("DefeatLogo"));
				UWidget* VictoryLogo = EndMenuWidget->GetWidgetFromName(TEXT("VictoryLogo"));

				if (DefeatLogo)
				{
					DefeatLogo->SetVisibility(ESlateVisibility::Hidden);
				}

				if (VictoryLogo)
				{
					VictoryLogo->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}

void AShadow_of_the_DesertGameState::BeginPlay()
{
	Super::BeginPlay();

}



