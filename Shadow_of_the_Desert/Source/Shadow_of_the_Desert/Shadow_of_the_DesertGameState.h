// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Shadow_of_the_DesertGameState.generated.h"

/**
 * 
 */
UCLASS()
class SHADOW_OF_THE_DESERT_API AShadow_of_the_DesertGameState : public AGameState
{
	GENERATED_BODY()
public:
	AShadow_of_the_DesertGameState();

	// ���� ���� Ƚ��, ���� �� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 KillEnemyCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 AllEnemyCount;

	// �� ���� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 MinSpawnNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 MaxSpawnNum;

	// ������ �Ͻ����� �������� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
	bool bIsPaused;

	//�Ͻ�����â
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	//���� ����â
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> EndMenuWidgetClass;

	// HUD ���� ���� �� ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	// ����, �÷��̾� ��� ����
	bool bIsBossDead;
	bool bIsPlayerDead;
	

	FTimerHandle GameTimerHandle;
	
	FTimerHandle EnemyTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	float LocalElapsedTime;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void LocalStartGame();

	// ���� �Ͻ����� ó��
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalPauseGame();

	// ���� �簳 ó��
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalResumeGame();

	// ���� ���� ó��
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalResetGame();


	void SetHUDVisibility(bool bVisible);
	void SpawnBoss();
	void KillEnemy(int32 Score);	// ���� �׾�����
	void EnemySpawn();
	void TimerUpdate();
	void UpdateHUD();
	void GameEnd(FString Result);	// ���� ����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bIsTimerRunning;
	UUserWidget* PauseMenuWidget;
	UUserWidget* EndMenuWidget;
	UUserWidget* HUDWidget;
};
