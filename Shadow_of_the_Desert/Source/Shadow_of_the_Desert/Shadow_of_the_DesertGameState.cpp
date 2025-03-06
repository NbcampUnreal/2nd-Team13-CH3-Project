// Fill out your copyright notice in the Description page of Project Settings.


#include "Shadow_of_the_DesertGameState.h"
#include "Shadow_of_the_DesertGameMode.h"
#include "Shadow_of_the_DesertGameInstance.h"
#include "Shadow_of_the_DesertCharacter.h"
#include "EnemySpawner.h"
#include "Enemy/EnemyCharacterAi.h"
#include "Weapon/WeaponBase.h"
#include "Player_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameModeBase.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

AShadow_of_the_DesertGameState::AShadow_of_the_DesertGameState()
{
	RoundScore = 0;
	WaveCount = 0;
	KillEnemyCount = 0;
	AllEnemyCount = 0;
	MinSpawnNum = 2;
	MaxSpawnNum = 3;
	PreviousMinutes = 0;
	LocalElapsedTime = 0.0f;
	bIsGameEnded = false;
	bIsTimesUp = false;
	bIsPlayerDead = false;
	bIsTimerRunning = false;
	bIsPaused = false;
}


void AShadow_of_the_DesertGameState::LocalStartGame()
{
	// ���� ���� ���� Ȯ�� �� �ð� ����
	if (bIsGameEnded)
	{
		bIsGameEnded = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
	}

	// ���� ���۽� �ʱ�ȭ
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

void AShadow_of_the_DesertGameState::SetDamage(int32 Damage)
{
	if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SOTDInstance->TotalDamageDealt += Damage;
	}
}

void AShadow_of_the_DesertGameState::SetTakenDamage(int32 Damage)
{
	if (UShadow_of_the_DesertGameInstance* SOTDInstance = Cast<UShadow_of_the_DesertGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		SOTDInstance->TotalDamageTaken += Damage;
	}
}

void AShadow_of_the_DesertGameState::CheckTimesUp()
{
	//���ѽð� ����(���߿� ������ ����� �װ� üũ�ϴ¿뵵�ε� ����)
	bIsTimesUp = true;
}

void AShadow_of_the_DesertGameState::KillEnemy(int32 Score)
{
	RoundScore += Score;
	KillEnemyCount++;

	//���� ���� ���� ���ѽð� ���� �� ��� ���� óġ ��
	if (bIsTimesUp && KillEnemyCount >= AllEnemyCount)
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
	WaveCount++;
	// ��Ƽ�� �ð� ����
	if (LocalElapsedTime >= 180.0f)
	{
		// �ð� ���� üũ �� ���� ���� ���߱�
		CheckTimesUp();
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
		// 웨이브 표시
		UTextBlock* CurrentWaveBlock = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CurrentWaveBlock")));
		if (CurrentWaveBlock)
		{
			int32 WaveNum = WaveCount;
			CurrentWaveBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), WaveNum)));
		}

		// 인게임 시간 표기
		UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("WaveTimerTextBlock")));
		if (TimeText)
		{
			int32 Minutes = static_cast<int32>(LocalElapsedTime) / 60;  // 분 계산
			int32 Seconds = static_cast<int32>(LocalElapsedTime) % 60;  // 초 계산

			TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)));
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
				FString DamageText;

				if (PlayerDamage >= 1000) // 1000 이상이면 "1.0K" 형식으로 표시
				{
					DamageText = FString::Printf(TEXT("%.1fK"), PlayerDamage / 1000.0f);
				}
				else // 1000 미만이면 그대로 숫자로 표시
				{
					DamageText = FString::Printf(TEXT("%d"), PlayerDamage);
				}

				PlayerDamageText->SetText(FText::FromString(DamageText));
			}
		}

		// 플레이어 무기 상태 업데이트
		if (AShadow_of_the_DesertCharacter* PlayerCharacter = Cast<AShadow_of_the_DesertCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
		{
			// 각 무기에 대한 UImage 가져오기
			UImage* RifleImage = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("AKImage")));
			UImage* SniperImage = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("SniperRifleImage")));
			UImage* RocketImage = Cast<UImage>(HUDWidget->GetWidgetFromName(TEXT("RPGImage")));

			// 총알 표기 상자
			UTextBlock* CurrentAmmoTextBox = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("CurrentAmmoTextBox")));
			UTextBlock* MaxAmmoTextBlock = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("MaxAmmoTextBlock")));
			
			// 모든 무기 UI를 숨김
			if (RifleImage) RifleImage->SetVisibility(ESlateVisibility::Hidden);
			if (SniperImage) SniperImage->SetVisibility(ESlateVisibility::Hidden);
			if (RocketImage) RocketImage->SetVisibility(ESlateVisibility::Hidden);
			
			// 현재 무기의 정보 가져오기
			AWeaponBase* CurrentWeapon = PlayerCharacter->GetEquippedWeapon();

			// 현재 장착된 무기에 따라 맞는 HUD 출력
			if (PlayerCharacter->Ues_Rifle_now && RifleImage&&CurrentWeapon)
			{
				RifleImage->SetVisibility(ESlateVisibility::Visible);

				int32 CurrentAmmo = 0;
				int32 MaxAmmo = 0;
				CurrentAmmoTextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentAmmo)));
				MaxAmmoTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), MaxAmmo)));
			}
			else if (PlayerCharacter->Ues_Sniper_now&&SniperImage&&CurrentWeapon)
			{
				SniperImage->SetVisibility(ESlateVisibility::Visible);

				int32 CurrentAmmo = 0;
				int32 MaxAmmo = 0;
				CurrentAmmoTextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentAmmo)));
				MaxAmmoTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), MaxAmmo)));
			}
			else if (PlayerCharacter->Ues_Rocket_now&&RocketImage&&CurrentWeapon)
			{
				RocketImage->SetVisibility(ESlateVisibility::Visible);

				int32 CurrentAmmo = 0;
				int32 MaxAmmo = 0;
				CurrentAmmoTextBox->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentAmmo)));
				MaxAmmoTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), MaxAmmo)));
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
				static float CurrentHealth = MyCharacter->GetHelth(); // 이전 프레임에서의 체력 저장
				int32 PlayerHP = MyCharacter->GetHelth();
				int32 MaxHP = MyCharacter->GetMaxHelth();

				// 체력 보간 (부드럽게 감소)
				float InterpSpeed = 5.0f;  // 보간 속도 조절 가능
				CurrentHealth = FMath::FInterpTo(CurrentHealth, PlayerHP, GetWorld()->GetDeltaSeconds(), InterpSpeed);

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
					float HPPercent = CurrentHealth / MaxHP;
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

	// 총기 제거
	TArray<AActor*> FoundWeapons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeaponBase::StaticClass(), FoundWeapons);

	for (AActor* Weapons : FoundWeapons)
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(Weapons);
		if (Weapon)
		{
			Weapon->Destroy();
		}
	}

	// 내용 초기화
	RoundScore = 0;
	WaveCount = 0;
	KillEnemyCount = 0;
	AllEnemyCount = 0;
	MinSpawnNum = 2;
	MaxSpawnNum = 3;
	PreviousMinutes = 0;
	LocalElapsedTime = 0.0f;
	bIsTimesUp = false;
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
}

void AShadow_of_the_DesertGameState::LocalReStartGame()
{
	//초기화
	LocalResetGame();

	// 게임 시작
	LocalStartGame();
}

void AShadow_of_the_DesertGameState::GoMainMenu()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		PlayerController->SetPause(false);
	}

	// 타이머 정리
	GetWorldTimerManager().ClearTimer(GameTimerHandle);
	GetWorldTimerManager().ClearTimer(HUDUpdateTimerHandle);
	GetWorldTimerManager().ClearTimer(EnemyTimerHandle);

	// 초기화
	LocalResetGame();

	if (AShadow_of_the_DesertGameMode* GameMode = Cast<AShadow_of_the_DesertGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->ShowMainMenu();
	}
}

void AShadow_of_the_DesertGameState::GameEnd(FString Result)
{
	// 게임 종료 체크
	bIsGameEnded = true;

	// 메뉴를 제외한 모든것 정지
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);

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

					TimeText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds)));
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



