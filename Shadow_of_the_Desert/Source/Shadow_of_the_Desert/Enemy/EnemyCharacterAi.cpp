// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterAi.h"
#include "EnemyAIController.h"

// Sets default values
AEnemyCharacterAi::AEnemyCharacterAi()
{
 	
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}