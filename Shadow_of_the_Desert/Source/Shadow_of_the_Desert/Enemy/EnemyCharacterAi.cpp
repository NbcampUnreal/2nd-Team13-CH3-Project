// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterAi.h"
#include "EnemyAIController.h"

// Sets default values
AEnemyCharacterAi::AEnemyCharacterAi()
{ 	
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	maxHp = 100.0f;
	currentHp = 100.0f;
	attackPower = 10.0f;

	attackAnim = nullptr;

	attackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	attackCollision->SetupAttachment(RootComponent);

	attackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	attackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	attackCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEnemyCharacterAi::EnemyAttack()
{
	attackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	FTimerHandle attackTimer;
	GetWorld()->GetTimerManager().SetTimer(attackTimer, [this]()
		{
			attackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}, 0.5f, false);

	PlayAnimation();
	ApplyDamage();
}

void AEnemyCharacterAi::ApplyDamage()
{
	TArray<AActor*> overlapActors;
	attackCollision->GetOverlappingActors(overlapActors);

	for (AActor* Actor : overlapActors)
	{
		if (Actor && Actor->IsA(APawn::StaticClass()))
		{
			APawn* PlayerPawn = Cast<APawn>(Actor);
			if (PlayerPawn)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Player Hit");
			}
		}
	}
}

void AEnemyCharacterAi::PlayAnimation()
{
	if (attackAnim && GetMesh())
	{
		GetMesh()->PlayAnimation(attackAnim, false);
	}
}

void AEnemyCharacterAi::EnemyTakeDamage(const int32 damage)
{
	currentHp -= damage;
	if (currentHp <= 0)
	{
		FTimerHandle delayTime;
		GetWorld()->GetTimerManager().SetTimer(delayTime, this, &AEnemyCharacterAi::EnemyDespawn, 5.0f, false);
	}
}

void AEnemyCharacterAi::EnemyDespawn()
{
	Destroy();
}