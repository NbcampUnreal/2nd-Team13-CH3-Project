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
	SetingHUD();
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
		true);

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
		true);
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
		PauseMenuWidget->RemoveFromParent();
		PauseMenuWidget = nullptr;

		// HUD 다시 표시
		SetHUDVisibility(true);
	}
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
	RoundScore += Score;
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

		// 분마다 난이도 올라가게끔?
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
	// 현재 사용 중인 GameMode 가져오기
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		// GameMode에서 설정한 HUDClass(BP_CustomHUD)를 가져오기
		TSubclassOf<AHUD> HUDClass = GameMode->HUDClass;
		if (HUDClass)
		{
			// BP_CustomHUD를 수동으로 생성
			HUDInstance = GetWorld()->SpawnActor<AHUD>(HUDClass);
			if (HUDInstance)
			{
				// BP_CustomHUD에서 HUDWidgetClass 가져오기
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
		// 진행 시간 표시
		UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText")));
		if (TimeText)
		{
			int32 Minutes = static_cast<int32>(LocalElapsedTime) / 60;  // 분 계산
			int32 Seconds = static_cast<int32>(LocalElapsedTime) % 60;  // 초 계산

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

		// 플레이어 데미지
		if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			UTextBlock* PlayerDamageText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("PlayerDamageText")));
			if (PlayerDamageText)
			{
				int32 PlayerDamage = SOTDInstance->TotalDamageDealt;
				PlayerDamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerDamage)));
			}
		}

		// 현재 플레이어 컨트롤러를 통해 조종 중인 캐릭터 가져오기
		ACharacter* PlayerCharacter = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (PlayerCharacter)
		{
			AShadow_of_the_DesertCharacter* MyCharacter = Cast<AShadow_of_the_DesertCharacter>(PlayerCharacter);

			if (MyCharacter)
			{
				//체력
				int32 PlayerHP = MyCharacter->GetHelth();
				int32 MaxHP = MyCharacter->GetMaxHelth();

				UTextBlock* PlayerHealthTextBlock = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("PlayerHealthTextBlock")));
				UTextBlock* PlayerMaxHealthTextBlock = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("PlayerMaxHealthTextBlock")));
				if (PlayerHealthTextBlock && PlayerMaxHealthTextBlock)
				{
					PlayerHealthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), PlayerHP)));
					PlayerMaxHealthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), MaxHP)));
				}
				// 체력바
				UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("PlayerHealthProgressBar")));
				if (HealthBar)
				{
					float HPPercent = static_cast<float>(PlayerHP) / MaxHP;
					HealthBar->SetPercent(HPPercent);
				}
				
				//경험치바

				/*UProgressBar* ExpBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("LevelProgressBar")));
				if (ExpBar)
				{
					경험치로 바꿔줘야됨
					float ExpPercent = static_cast<float>(PlayerHP) / MaxHP;
					ExpBar->SetPercent(HPPercent);
				}*/
			}
		}
	}
}

void AShadow_of_the_DesertGameState::LocalResetGame()
{
	// 현재 실행 중인 맵 이름 가져오기
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // 필요할 경우 접두사 제거

	// 현재 맵 다시 로드
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}

void AShadow_of_the_DesertGameState::LocalReStartGame()
{
	if (EndMenuWidget)
	{
		EndMenuWidget->RemoveFromParent();
		EndMenuWidget = nullptr; // 포인터 초기화
	}

	// 현재 존재하는 적들만 제거
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

	// 게임 관련 변수 초기화
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


	// 플레이어 상태 초기화
	AShadow_of_the_DesertGameMode* GameMode = Cast<AShadow_of_the_DesertGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->RestartGame();
	}

	// 타이머 리셋
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().ClearTimer(EnemyTimerHandle);

	// 게임 시작
	LocalStartGame();
}

void AShadow_of_the_DesertGameState::GameEnd(FString Result)
{
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);

	// 게임 종료시 HUD 제거
	if (HUDWidget)
	{
		HUDWidget->RemoveFromParent();
		HUDWidget = nullptr; // 포인터 초기화
	}

	// 게임 종료 UI 생성 및 표시
	if (EndMenuWidgetClass && EndMenuWidget == nullptr)
	{
		EndMenuWidget = CreateWidget<UUserWidget>(GetWorld(), EndMenuWidgetClass);
		if (EndMenuWidget)
		{
			EndMenuWidget->AddToViewport();

			// UI 모드로 변경
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(EndMenuWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(InputMode);
			}

			// 승리 패배시 바뀌는값은 여기다가
			if (Result == "Clear")
			{
				UWidget* DefeatLogo = EndMenuWidget->GetWidgetFromName(TEXT("DefeatLogo"));
				UWidget* VictoryLogo = EndMenuWidget->GetWidgetFromName(TEXT("VictoryLogo"));

				UWidget* DefeatBorder = EndMenuWidget->GetWidgetFromName(TEXT("DefeatBorder"));
				UWidget* VictoryBorder = EndMenuWidget->GetWidgetFromName(TEXT("VictoryBorder"));

				// Logo는 여기
				if (DefeatLogo && VictoryLogo)
				{
					DefeatLogo->SetVisibility(ESlateVisibility::Hidden);
					VictoryLogo->SetVisibility(ESlateVisibility::Visible);
				}

				// Border는 여기
				if (DefeatBorder && VictoryBorder)
				{
					DefeatBorder->SetVisibility(ESlateVisibility::Hidden);
					VictoryBorder->SetVisibility(ESlateVisibility::Visible);
				}
			}

			if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
			{
				
				// 진행 시간 표시
				UTextBlock* TimeText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("TimeText")));
				if (TimeText)
				{
					int32 Minutes = static_cast<int32>(LocalElapsedTime) / 60;  // 분 계산
					int32 Seconds = static_cast<int32>(LocalElapsedTime) % 60;  // 초 계산

					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %02d:%02d"), Minutes, Seconds)));
				}

				// 생존시간 점수 분당 1점
				UTextBlock* TimeScoreText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("TimeScoreText")));
				if (TimeScoreText)
				{
					int32 TimeScore = ElapsedTime;
					SOTDInstance->TotalScore += TimeScore;
					TimeScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TimeScore)));
				}

				//처치 수
				UTextBlock* KillCountTextBlock = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("KillCountTextBlock")));
				if (KillCountTextBlock)
				{
					KillCountTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), KillEnemyCount)));
				}

				//점수
				UTextBlock* ScoreText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("ScoreText")));
				if (ScoreText)
				{
					int32 Score = RoundScore;
					SOTDInstance->TotalScore += Score;
					ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Score)));
				}

				// 총 데미지(입힌 피해)
				UTextBlock* DamageText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("DamageText")));
				UTextBlock* DamageScoreText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("DamageScoreText")));
				if (DamageText && DamageScoreText)
				{
					int32 TotalDamage = SOTDInstance->TotalDamageDealt;
					SOTDInstance->TotalScore += TotalDamage;
					DamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalDamage)));
					DamageScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalDamage)));
				}

				// 총 입은 피해
				UTextBlock* TakenDamageText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("TakenDamageText")));
				UTextBlock* TakenDamageScoreText = Cast<UTextBlock>(EndMenuWidget->GetWidgetFromName(TEXT("TakenDamageScoreText")));
				if (TakenDamageText && TakenDamageScoreText)
				{
					int32 TotalTakenDamage = SOTDInstance->TotalDamageTaken;
					SOTDInstance->TotalScore += TotalTakenDamage;
					TakenDamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalTakenDamage)));
					TakenDamageScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TotalTakenDamage)));
				}

				// 총점
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



