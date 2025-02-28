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

	// ���� ���۽� HUD ���� ���� �� �߰�
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	//UI ������Ʈ
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AShadow_of_the_DesertGameState::UpdateHUD,
		0.1f,
		false);

	// ���θ޴� �ݱ�
	if (AShadow_of_the_DesertGameMode* GameMode = Cast<AShadow_of_the_DesertGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->CloseMainMenu();
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
}

void AShadow_of_the_DesertGameState::LocalPauseGame()
{
	// Pause Menu ���� �� ȭ�鿡 �߰�
	if (PauseMenuWidgetClass && PauseMenuWidget == nullptr)
	{
		PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
		if (PauseMenuWidget)
		{
			// HUD �����
			SetHUDVisibility(false);

			PauseMenuWidget->AddToViewport();

			// UI ���� ����
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

		// ���� ���� ����
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}

	// Pause Menu ����
	if (PauseMenuWidget)
	{
		PauseMenuWidget->RemoveFromViewport();
		PauseMenuWidget = nullptr;

		// HUD �ٽ� ǥ��
		SetHUDVisibility(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Game Resumed"));
}

void AShadow_of_the_DesertGameState::SetHUDVisibility(bool bVisible)
{
	if (HUDWidget)
	{
		// HUD�� ���ü��� ����
		ESlateVisibility VisibilityState = bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		HUDWidget->SetVisibility(VisibilityState);
	}
}

void AShadow_of_the_DesertGameState::SpawnBoss()
{
	//���� ���� �ڵ� �ۼ�
	bIsBossDead = true;
}

void AShadow_of_the_DesertGameState::KillEnemy(int32 Score)
{
	if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SOTDInstance->TotalScore += Score;
	}

	KillEnemyCount++;

	//���� ���� ���� ��� ���Ͷ� ������ ���̸�
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
	// ���� �����ð�
	if (LocalElapsedTime >= 60.0f)
	{
		// ���� ��ȯ �� ���� ���� ���߱�(������ �����°� ���и�)
		SpawnBoss();
		GetWorldTimerManager().ClearTimer(EnemyTimerHandle);

		return;
	}

	// Min~Max ���� ���ڸ�ŭ ������Ű��
	int32 RandomSpawn = FMath::RandRange(MinSpawnNum, MaxSpawnNum);
	AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(this, AEnemySpawner::StaticClass()));

	// ������ ��ġ�Ǿ��ִ��� Ȯ��(���߿� ���� �Ǵ� �ڵ�?)
	if (!EnemySpawner)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawner not found"));
		return;
	}
	// ���� ����
	for (size_t i = 0; i < RandomSpawn; i++)
	{
		EnemySpawner->EnemySpawn();
		AllEnemyCount++;
	}
}

void AShadow_of_the_DesertGameState::TimerUpdate()
{
	// ������ �Ͻ����� ���� �ƴ϶�� ��� �ð� 1�ʾ� ����
	if (!bIsPaused)
	{
		LocalElapsedTime += 1.0f;
		UE_LOG(LogTemp, Warning, TEXT("Elapsed Time: %.1f"), LocalElapsedTime);

		// �и��� ���̵� �ö󰡰Բ�?
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
	//	// ���� ǥ��
	//	UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText")));
	//	if (ScoreText)
	//	{
	//		if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
	//		{
	//			int32 Score = SOTDInstance->TotalScore;
	//			ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
	//		}
	//	}

	//	// ���� �ð� ǥ��
	//	UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText")));
	//	if (TimeText)
	//	{
	//		int32 Minutes = static_cast<int32>(LocalElapsedTime) / 60;  // �� ���
	//		int32 Seconds = static_cast<int32>(LocalElapsedTime) % 60;  // �� ���

	//		TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %02d:%02d"), Minutes, Seconds)));
	//	}

	//	// ���� �÷��̾� ��Ʈ�ѷ��� ���� ���� ���� ĳ���� ��������
	//	ACharacter* PlayerCharacter = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	//	if (PlayerCharacter)
	//	{
	//		AShadow_of_the_DesertCharacter* MyCharacter = Cast<AShadow_of_the_DesertCharacter>(PlayerCharacter);

	//		if (MyCharacter)
	//		{
	//			int32 PlayerHP = MyCharacter->Health;
	//			int32 MaxHP = MyCharacter->MaxHealth;

	//			// ü�� ������ HUD�� �ݿ�
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
	// ���� ���� ���� �� �̸� ��������
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // �ʿ��� ��� ���λ� ����

	// ���� �� �ٽ� �ε�
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}

void AShadow_of_the_DesertGameState::GameEnd(FString Result)
{
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);

	// ���� ���� UI ���� �� ǥ��
	if (EndMenuWidgetClass && EndMenuWidget == nullptr)
	{
		EndMenuWidget = CreateWidget<UUserWidget>(GetWorld(), EndMenuWidgetClass);
		if (EndMenuWidget)
		{
			EndMenuWidget->AddToViewport();

			// UI ���� ���� (���콺 Ȱ��ȭ)
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(EndMenuWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(InputMode);
			}

			// �����ǰ����� ����ٰ�

			// �¸� �й�� �ٲ�°��� ����ٰ�
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



