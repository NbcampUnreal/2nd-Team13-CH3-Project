// Fill out your copyright notice in the Description page of Project Settings.


#include "Shadow_of_the_DesertGameState.h"
#include "Shadow_of_the_DesertGameMode.h"
#include "Shadow_of_the_DesertGameInstance.h"
#include "Shadow_of_the_DesertCharacter.h"
#include "Enemy/EnemyCharacterAi.h"
#include "EnemySpawner.h"
#include "Player_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameModeBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

AShadow_of_the_DesertGameState::AShadow_of_the_DesertGameState()
{
	RoundScore = 0;
	KillEnemyCount = 0;
	AllEnemyCount = 0;
	MinSpawnNum = 10;
	MaxSpawnNum = 15;
	PreviousMinutes = 0;
	LocalElapsedTime = 0.0f;
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

	// ���� ���۽� HUD ���� ���� �� �߰�
	SetingHUD();
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
		true);

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
		true);
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
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;

		// HUD �ٽ� ǥ��
		SetHUDVisibility(true);
	}
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
	RoundScore += Score;
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

		// �и��� ���̵� �ö󰡰Բ�?
		int CurrentMinutes = FMath::FloorToInt(LocalElapsedTime / 60.0f);

		if (CurrentMinutes > PreviousMinutes)
		{
			MinSpawnNum += 2;
			MaxSpawnNum += 2;

			PreviousMinutes = CurrentMinutes;
		}
	}
}

void AShadow_of_the_DesertGameState::SetingHUD()
{
	// ���� ��� ���� GameMode ��������
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		// GameMode���� ������ HUDClass(BP_CustomHUD)�� ��������
		TSubclassOf<AHUD> HUDClass = GameMode->HUDClass;
		if (HUDClass)
		{
			// BP_CustomHUD�� �������� ����
			HUDInstance = GetWorld()->SpawnActor<AHUD>(HUDClass);
			if (HUDInstance)
			{
				// BP_CustomHUD���� HUDWidgetClass ��������
				HUDWidgetClass = nullptr;
				UObject* HUDObject = Cast<UObject>(HUDInstance);

				static FName WidgetClassVarName = TEXT("HUDWidgetClass");
				FProperty* Property = HUDObject->GetClass()->FindPropertyByName(WidgetClassVarName);

				if (Property)
				{
					FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property);
					if (ObjectProperty)
					{
						UObject* RawObject = ObjectProperty->GetPropertyValue_InContainer(HUDObject);
						if (RawObject)
						{
							HUDWidgetClass = RawObject->GetClass();
						}
					}
				}
			}
		}
	}
}

void AShadow_of_the_DesertGameState::UpdateHUD()
{
	if (HUDWidget)
	{
		// ���� �ð� ǥ��
		UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText")));
		if (TimeText)
		{
			int32 Minutes = static_cast<int32>(LocalElapsedTime) / 60;  // �� ���
			int32 Seconds = static_cast<int32>(LocalElapsedTime) % 60;  // �� ���

			TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %02d:%02d"), Minutes, Seconds)));
		}

		UTextBlock* EnemyCountText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("EnemyCountText")));
		if (EnemyCountText)
		{
			int32 EnemyCount = AllEnemyCount - KillEnemyCount;

			EnemyCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), EnemyCount)));
		}

		UTextBlock* EnemyKillCountText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("EnemyKillCountText")));
		if (EnemyKillCountText)
		{
			EnemyKillCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), KillEnemyCount)));
		}

		// �÷��̾� ������
		if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			UTextBlock* PlayerDamageText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("PlayerDamageText")));
			if (PlayerDamageText)
			{
				int32 PlayerDamage = SOTDInstance->TotalDamageDealt;
				PlayerDamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerDamage)));
			}
		}

		// ���� �÷��̾� ��Ʈ�ѷ��� ���� ���� ���� ĳ���� ��������
		ACharacter* PlayerCharacter = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (PlayerCharacter)
		{
			AShadow_of_the_DesertCharacter* MyCharacter = Cast<AShadow_of_the_DesertCharacter>(PlayerCharacter);

			if (MyCharacter)
			{
				//ü��
				int32 PlayerHP = MyCharacter->GetHelth();
				int32 MaxHP = MyCharacter->GetMaxHelth();

				UTextBlock* PlayerHealthTextBlock = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("PlayerHealthTextBlock")));
				UTextBlock* PlayerMaxHealthTextBlock = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("PlayerMaxHealthTextBlock")));
				if (PlayerHealthTextBlock && PlayerMaxHealthTextBlock)
				{
					PlayerHealthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerHP)));
					PlayerMaxHealthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), MaxHP)));
				}
				// ü�¹�
				UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("PlayerHealthProgressBar")));
				if (HealthBar)
				{
					float HPPercent = static_cast<float>(PlayerHP) / MaxHP;
					HealthBar->SetPercent(HPPercent);
				}
				
				//����ġ��

				/*UProgressBar* ExpBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("LevelProgressBar")));
				if (ExpBar)
				{
					����ġ�� �ٲ���ߵ�
					float ExpPercent = static_cast<float>(PlayerHP) / MaxHP;
					ExpBar->SetPercent(HPPercent);
				}*/
			}
		}
	}
}

void AShadow_of_the_DesertGameState::LocalResetGame()
{
	// ���� ���� ���� �� �̸� ��������
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // �ʿ��� ��� ���λ� ����

	// ���� �� �ٽ� �ε�
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}

void AShadow_of_the_DesertGameState::LocalReStartGame()
{
	if (EndMenuWidget)
	{
		EndMenuWidget->RemoveFromParent();
		EndMenuWidget = nullptr; // ������ �ʱ�ȭ
	}

	// ���� �����ϴ� ���鸸 ����
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacterAi::StaticClass(), FoundEnemies);

	for (AActor* Enemy : FoundEnemies)
	{
		AEnemyCharacterAi* EnemyAI = Cast<AEnemyCharacterAi>(Enemy);
		if (EnemyAI)
		{
			EnemyAI->Destroy();
		}
	}

	// ���� ���� ���� �ʱ�ȭ
	RoundScore = 0;
	KillEnemyCount = 0;
	AllEnemyCount = 0;
	MinSpawnNum = 10;
	MaxSpawnNum = 15;
	PreviousMinutes = 0;
	LocalElapsedTime = 0.0f;
	bIsBossDead = false;
	bIsPlayerDead = false;
	bIsTimerRunning = false;
	bIsPaused = false;


	// �÷��̾� ���� �ʱ�ȭ
	AShadow_of_the_DesertGameMode* GameMode = Cast<AShadow_of_the_DesertGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->RestartGame();
	}

	// Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().ClearTimer(EnemyTimerHandle);

	// ���� ����
	LocalStartGame();
}

void AShadow_of_the_DesertGameState::GameEnd(FString Result)
{
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);

	// ���� ����� HUD ����
	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr; // ������ �ʱ�ȭ
	}

	// ���� ���� UI ���� �� ǥ��
	if (EndMenuWidgetClass && EndMenuWidget == nullptr)
	{
		EndMenuWidget = CreateWidget<UUserWidget>(GetWorld(), EndMenuWidgetClass);
		if (EndMenuWidget)
		{
			EndMenuWidget->AddToViewport();

			// UI ���� ����
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(EndMenuWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(InputMode);
			}

			// �¸� �й�� �ٲ�°��� ����ٰ�
			if (Result == "Clear")
			{
				UWidget* DefeatLogo = EndMenuWidget->GetWidgetFromName(TEXT("DefeatLogo"));
				UWidget* VictoryLogo = EndMenuWidget->GetWidgetFromName(TEXT("VictoryLogo"));

				UWidget* DefeatBorder = EndMenuWidget->GetWidgetFromName(TEXT("DefeatBorder"));
				UWidget* VictoryBorder = EndMenuWidget->GetWidgetFromName(TEXT("VictoryBorder"));

				// Logo�� ����
				if (DefeatLogo && VictoryLogo)
				{
					DefeatLogo->SetVisibility(ESlateVisibility::Hidden);
					VictoryLogo->SetVisibility(ESlateVisibility::Visible);
				}

				// Border�� ����
				if (DefeatBorder && VictoryBorder)
				{
					DefeatBorder->SetVisibility(ESlateVisibility::Hidden);
					VictoryBorder->SetVisibility(ESlateVisibility::Visible);
				}
			}

			if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				
				// ���� �ð� ǥ��
				UTextBlock* TimeText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("TimeText")));
				if (TimeText)
				{
					int32 Minutes = static_cast<int32>(LocalElapsedTime) / 60;  // �� ���
					int32 Seconds = static_cast<int32>(LocalElapsedTime) % 60;  // �� ���

					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %02d:%02d"), Minutes, Seconds)));
				}

				// �����ð� ���� �д� 1��
				UTextBlock* TimeScoreText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("TimeScoreText")));
				if (TimeScoreText)
				{
					int32 TimeScore = ElapsedTime;
					SOTDInstance->TotalScore += TimeScore;
					TimeScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TimeScore)));
				}

				//óġ ��
				UTextBlock* KillCountTextBlock = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("KillCountTextBlock")));
				if (KillCountTextBlock)
				{
					KillCountTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), KillEnemyCount)));
				}

				//����
				UTextBlock* ScoreText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("ScoreText")));
				if (ScoreText)
				{
					int32 Score = RoundScore;
					SOTDInstance->TotalScore += Score;
					ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
				}

				// �� ������(���� ����)
				UTextBlock* DamageText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("DamageText")));
				UTextBlock* DamageScoreText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("DamageScoreText")));
				if (DamageText && DamageScoreText)
				{
					int32 TotalDamage = SOTDInstance->TotalDamageDealt;
					SOTDInstance->TotalScore += TotalDamage;
					DamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalDamage)));
					DamageScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalDamage)));
				}

				// �� ���� ����
				UTextBlock* TakenDamageText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("TakenDamageText")));
				UTextBlock* TakenDamageScoreText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("TakenDamageScoreText")));
				if (TakenDamageText && TakenDamageScoreText)
				{
					int32 TotalTakenDamage = SOTDInstance->TotalDamageTaken;
					SOTDInstance->TotalScore += TotalTakenDamage;
					TakenDamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalTakenDamage)));
					TakenDamageScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalTakenDamage)));
				}

				// ����
				UTextBlock* TotalScoreText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("TotalScoreText")));
				if (TotalScoreText)
				{
					int32 TotalScore = SOTDInstance->TotalScore;
					TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalScore)));
				}
			}
		}
	}
}

void AShadow_of_the_DesertGameState::BeginPlay()
{
	Super::BeginPlay();

	
}



