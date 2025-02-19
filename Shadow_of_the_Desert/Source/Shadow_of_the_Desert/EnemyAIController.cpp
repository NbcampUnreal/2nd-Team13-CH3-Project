// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyCharacterAi.h"

void AEnemyAIController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	MoveToCurrentPatrolPoint();
}

void AEnemyAIController::MoveToCurrentPatrolPoint()
{
	AEnemyCharacterAi* enemyChar = Cast<AEnemyCharacterAi>(GetPawn());
	if (!enemyChar)
	{
		return;
	}

	if (enemyChar->patrolPoints.Num() == 0)
	{
		return;
	}

	MoveToActor(
		enemyChar->patrolPoints[currentPatrolIdx],
		5.0f,
		true,
		true,
		false,
		nullptr,
		true
	);

	currentPatrolIdx = (currentPatrolIdx + 1) % enemyChar->patrolPoints.Num();
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID requestId, const FPathFollowingResult& result)
{
	Super::OnMoveCompleted(requestId, result);

	if (result.Code == EPathFollowingResult::Success)
	{
		MoveToCurrentPatrolPoint();
	}
}