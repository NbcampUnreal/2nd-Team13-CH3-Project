// Fill out your copyright notice in the Description page of Project Settings.


#include "Shadow_of_the_DesertGameInstance.h"
#include "Save/SOTDSaveGame.h"
#include "Kismet/GameplayStatics.h"

UShadow_of_the_DesertGameInstance::UShadow_of_the_DesertGameInstance()
{
	TotalScore = 0;
	TotalDamageDealt = 0.0f;
	TotalDamageTaken = 0.0f;
}

void UShadow_of_the_DesertGameInstance::AddScore(int32 Score)
{
	TotalScore += Score;
}

void UShadow_of_the_DesertGameInstance::AddDamageDealt(float Damage)
{
	TotalDamageDealt += Damage;
}

void UShadow_of_the_DesertGameInstance::AddDamageTaken(float Damage)
{
	TotalDamageTaken += Damage;
}

void UShadow_of_the_DesertGameInstance::SaveSensitivity(float InSensitivity)
{
	USOTDSaveGame* SaveGameObject = Cast<USOTDSaveGame>(UGameplayStatics::CreateSaveGameObject(USOTDSaveGame::StaticClass()));
	if (SaveGameObject)
	{
		SaveGameObject->SavedMouseSensitivity = InSensitivity;
		UGameplayStatics::SaveGameToSlot(SaveGameObject, TEXT("SettingsSlot"), 0);
	}
}

float UShadow_of_the_DesertGameInstance::LoadSensitivity()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("SettingsSlot"), 0))
	{
		USOTDSaveGame* Loaded = Cast<USOTDSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SettingsSlot"), 0));
		if (Loaded)
		{
			return Loaded->SavedMouseSensitivity;
		}
	}
	return 1.0f; // 저장된 게 없으면 기본값
}
