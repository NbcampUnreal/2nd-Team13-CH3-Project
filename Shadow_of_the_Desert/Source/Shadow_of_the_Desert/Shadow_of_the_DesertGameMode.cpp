// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shadow_of_the_DesertGameMode.h"
#include "Shadow_of_the_DesertCharacter.h"
#include "Shadow_of_the_DesertGameState.h"
#include "UObject/ConstructorHelpers.h"

AShadow_of_the_DesertGameMode::AShadow_of_the_DesertGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	//캐릭터 컨트롤러 설정
	PlayerControllerClass = AShadow_of_the_DesertCharacter::StaticClass();
	// GameState 설정
	GameStateClass = AShadow_of_the_DesertGameState::StaticClass();
}
