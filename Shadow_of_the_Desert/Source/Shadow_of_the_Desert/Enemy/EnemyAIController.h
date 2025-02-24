// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class SHADOW_OF_THE_DESERT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* inPawn) override;
	virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID requestId, const FPathFollowingResult& result) override;

protected:
//	int32 currentPatrolIdx = 0;
//	void MoveToCurrentPatrolPoint();
};
