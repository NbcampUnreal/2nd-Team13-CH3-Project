// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"

// Sets default values
APatrolPath::APatrolPath()
{
	PrimaryActorTick.bCanEverTick = false;

}

AActor* APatrolPath::GetWayPoint(int32 idx)const
{
	if (wayPoints.IsValidIndex(idx))
	{
		return wayPoints[idx];
	}
	return nullptr;
}

int32 APatrolPath::Num()const
{
	return wayPoints.Num();
}