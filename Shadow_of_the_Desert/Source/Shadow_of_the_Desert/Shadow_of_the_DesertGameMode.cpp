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
	//ĳ���� ��Ʈ�ѷ� ����
	PlayerControllerClass = AShadow_of_the_DesertCharacter::StaticClass();
	// GameState ����
	GameStateClass = AShadow_of_the_DesertGameState::StaticClass();
}
