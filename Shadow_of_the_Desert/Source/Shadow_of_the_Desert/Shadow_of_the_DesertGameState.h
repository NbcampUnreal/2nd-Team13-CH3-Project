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

	// 적을 죽인 횟수, 적의 총 수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 KillEnemyCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 AllEnemyCount;

	// 적 스폰 양
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 MinSpawnNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	int32 MaxSpawnNum;

	// 게임이 일시정지 상태인지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Time")
	bool bIsPaused;

	//일시정지창
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	//게임 종료창
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> EndMenuWidgetClass;

	

	// 제한 시간 지났는지, 플레이어 사망 여부
	bool bIsTimesUp;
	bool bIsPlayerDead;
	

	FTimerHandle GameTimerHandle;
	FTimerHandle EnemyTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	float LocalElapsedTime;
	int PreviousMinutes;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void LocalStartGame();

	// 게임 일시정지 처리
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalPauseGame();

	// 게임 재개 처리
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalResumeGame();

	// 게임 리셋 처리
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalResetGame();

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void LocalReStartGame();

	void SetDamage(int32 Damage);
	void SetTakenDamage(int32 Damage);
	void CheckTimesUp();
	void KillEnemy(int32 Score);	// 적을 죽었을때
	void EnemySpawn();
	void TimerUpdate();
	void SetHUDVisibility(bool bVisible);
	void SetingHUD();
	void UpdateHUD();
	void GameEnd(FString Result);	// 게임 종료

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	bool bIsTimerRunning;
	bool bIsGameEnded;
	int32 RoundScore;
	UUserWidget* PauseMenuWidget;
	UUserWidget* EndMenuWidget;

	// HUD 위젯
	AHUD* HUDInstance;
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UUserWidget* HUDWidget;
};
