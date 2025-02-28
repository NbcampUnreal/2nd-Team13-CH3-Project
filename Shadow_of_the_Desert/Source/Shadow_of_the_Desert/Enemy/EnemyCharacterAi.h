// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacterAi.generated.h"

UCLASS()
class SHADOW_OF_THE_DESERT_API AEnemyCharacterAi : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterAi();
	UFUNCTION(BlueprintCallable)
	void EnemyAttack();
	UFUNCTION(BlueprintCallable)
	void ApplyDamage();
	UFUNCTION(BlueprintCallable)
	void EnemyTakeDamage(const int32 damage);
	UFUNCTION(BlueprintCallable)
	void EnemyDespawn();

	UFUNCTION(BlueprintCallable)
	void PlayAttackAnimation();
	UFUNCTION(BlueprintCallable)
	void PlayDeadAnimation();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy|Status")
	int32 maxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	int32 currentHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Status")
	int32 attackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Animation")
	UAnimSequence* attackAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Animation")
	UAnimSequence* deadAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Collision")
	USphereComponent* attackCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Collision")
	UCapsuleComponent* hitBoxCollision;
};
