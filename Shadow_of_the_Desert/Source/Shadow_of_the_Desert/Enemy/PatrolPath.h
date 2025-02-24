// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class SHADOW_OF_THE_DESERT_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolPath();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Patrol")
	TArray<AActor*> wayPoints;

	AActor* GetWayPoint(int32 idx) const;

	int32 Num() const;
};
