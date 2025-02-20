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

	// �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	// �� ���� ���ط�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float TotalDamageDealt;
	// �� ���� ���ط�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float TotalDamageTaken;


	// �� ������ ������Ű�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddScore(int32 Score);

	// �� ���� ���ط��� ������Ű�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddDamageDealt(float Damage);

	// �� ���� ���ط��� ������Ű�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddDamageTaken(float Damage);
};
