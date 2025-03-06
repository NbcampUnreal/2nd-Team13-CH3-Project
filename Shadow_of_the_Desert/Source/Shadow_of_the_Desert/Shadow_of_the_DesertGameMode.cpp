// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shadow_of_the_DesertGameMode.h"
#include "Shadow_of_the_DesertCharacter.h"
#include "Shadow_of_the_DesertGameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AShadow_of_the_DesertGameMode::AShadow_of_the_DesertGameMode()
{
	// set default pawn class to our Blueprinted character base(/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter)
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerCharacterBPClass(TEXT("/Game/BluePrints/BP_Shadow_of_the_DesertCharacter.BP_Shadow_of_the_DesertCharacter_C"));
	if (PlayerCharacterBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerCharacterBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/BluePrints/BP_Player_Controller.BP_Player_Controller_C"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AGameState> GameStateBPClass(TEXT("/Game/BluePrints/BP_Shadow_of_the_DesertGameState.BP_Shadow_of_the_DesertGameState_C"));
	if (GameStateBPClass.Class != NULL)
	{
		GameStateClass = GameStateBPClass.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UObject> HUDBPClass(TEXT("/Game/BluePrints/BP_CustomHUD.BP_CustomHUD_C"));
	if (HUDBPClass.Class != nullptr)
	{
		HUDClass = HUDBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/UI/Widgets/WBP_MainMenu.WBP_MainMenu_C"));
	if (MainMenuBPClass.Class != nullptr)
	{
		MainMenuWidgetClass = MainMenuBPClass.Class;
	}
}

void AShadow_of_the_DesertGameMode::BeginPlay()
{
	Super::BeginPlay();

	ShowMainMenu();
}

void AShadow_of_the_DesertGameMode::ShowMainMenu()
{
	if (MainMenuWidgetClass != nullptr)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenuWidget != nullptr)
		{
			MainMenuWidget->AddToViewport();

			if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->bShowMouseCursor = true;
				PC->SetInputMode(InputMode);
			}
		}
	}
}

void AShadow_of_the_DesertGameMode::CloseMainMenu()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		// UI 입력 모드 해제 후 게임 모드로 변경
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}

	// 메인 메뉴 제거
	if (MainMenuWidget != nullptr)
	{
		MainMenuWidget->RemoveFromViewport();
		MainMenuWidget = nullptr;  // UI가 완전히 제거되었음을 표시
	}
}

void AShadow_of_the_DesertGameMode::RestartGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) return;

	// 기존 플레이어 제거
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (PlayerPawn)
	{
		PlayerPawn->Destroy(); // 기존 캐릭터 삭제
	}

	// 플레이어 리스폰
	RestartPlayer(PlayerController); // 자동으로 PlayerStart에서 리스폰
}
