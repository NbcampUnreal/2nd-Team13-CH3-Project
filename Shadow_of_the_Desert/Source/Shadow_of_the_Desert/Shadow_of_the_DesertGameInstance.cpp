// Fill out your copyright notice in the Description page of Project Settings.


#include "Shadow_of_the_DesertGameInstance.h"

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
