// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Shadow_of_the_DesertGameState.generated.h"

class AHUD;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 MinSpawnNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
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

	

	// ���� �ð� ��������, �÷��̾� ��� ����
	bool bIsTimesUp;
	bool bIsPlayerDead;
	

	FTimerHandle GameTimerHandle;
	FTimerHandle EnemyTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	float LocalElapsedTime;
	int PreviousMinutes;

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

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalReStartGame();

	void SetDamage(int32 Damage);
	void SetTakenDamage(int32 Damage);
	void CheckTimesUp();
	void KillEnemy(int32 Score);	// ���� �׾�����
	void EnemySpawn();
	void TimerUpdate();
	void SetHUDVisibility(bool bVisible);
	void SetingHUD();
	void UpdateHUD();
	void GameEnd(FString Result);	// ���� ����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	bool bIsTimerRunning;
	bool bIsGameEnded;
	int32 RoundScore;
	UUserWidget* PauseMenuWidget;
	UUserWidget* EndMenuWidget;

	// HUD ����
	AHUD* HUDInstance;
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UUserWidget* HUDWidget;
};
