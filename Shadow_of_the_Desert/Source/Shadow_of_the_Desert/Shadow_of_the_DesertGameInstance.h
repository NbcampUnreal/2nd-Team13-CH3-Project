// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Shadow_of_the_DesertGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHADOW_OF_THE_DESERT_API UShadow_of_the_DesertGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UShadow_of_the_DesertGameInstance();

	// 총 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	// 총 입힌 피해량
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float TotalDamageDealt;
	// 총 입은 피해량
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float TotalDamageTaken;


	// 총 점수를 증가시키는 함수
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddScore(int32 Score);

	// 총 입힌 피해량을 증가시키는 함수
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddDamageDealt(float Damage);

	// 총 입은 피해량을 증가시키는 함수
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddDamageTaken(float Damage);
};
