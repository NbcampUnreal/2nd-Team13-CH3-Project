// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterAi.h"
#include "EnemyAIController.h"

// Sets default values
AEnemyCharacterAi::AEnemyCharacterAi()
{
 	
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
/*
// Called when the game starts or when spawned
void AEnemyCharacterAi::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacterAi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacterAi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

*/