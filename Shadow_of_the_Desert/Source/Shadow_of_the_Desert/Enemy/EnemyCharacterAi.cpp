// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterAi.h"
#include "EnemyAIController.h"
#include "../Shadow_of_the_DesertCharacter.h"

// Sets default values
AEnemyCharacterAi::AEnemyCharacterAi()
{ 	
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	maxHp = 100.0f;
	currentHp = 100.0f;
	attackPower = 10.0f;
	scorePoint = 100;
	attackSpeed = 0.5f;

	isDead = false;

	attackAnim = nullptr;
	deadAnim = nullptr;

	attackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	attackCollision->SetupAttachment(RootComponent);

	attackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	attackCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	attackCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	attackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacterAi::OnHit);

	hitBoxCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBoxCollision"));
	hitBoxCollision->SetupAttachment(RootComponent);

	hitBoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	hitBoxCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AEnemyCharacterAi::EnemyAttack()
{	
	FTimerHandle attackTimer;
	//FTimerHandle finishTimer;
	GetWorld()->GetTimerManager().SetTimer(attackTimer, [this]()
		{
			attackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}, attackSpeed, false);
	
	PlayAttackAnimation();
	//ApplyDamage();
	//GetWorld()->GetTimerManager().SetTimer(finishTimer, this, &AEnemyCharacterAi::DisableAttackCollision, 0.5f, false);
	DisableAttackCollision();
	/*
	AShadow_of_the_DesertGameState* gameState = Cast<AShadow_of_the_DesertGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		gameState->KillEnemy(scorePoint);
	}*/
}

void AEnemyCharacterAi::OnHit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor && OtherActor->IsA(AShadow_of_the_DesertCharacter::StaticClass()))
	{
		AShadow_of_the_DesertCharacter* playerChr = Cast<AShadow_of_the_DesertCharacter>(OtherActor);
		AAIController* aiCtl = Cast<AAIController>(GetController());
		if (playerChr&&aiCtl)
		{
//			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Player Hit");
//			FString dDM = FString::Printf(TEXT("Damage apply : %.2f"), GetAtkPower());
//			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, dDM);
//			UE_LOG(LogTemp, Warning, TEXT("%s"), *dDM);
			//playerChr->TakeDamage(attackPower);
			playerChr->TakeDamage(GetAtkPower(), FDamageEvent(), aiCtl, this);
		}
	}
}

void AEnemyCharacterAi::ActivateAttackCollision()
{
	attackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FTimerHandle attackTimer;
	GetWorld()->GetTimerManager().SetTimer(attackTimer, this, &AEnemyCharacterAi::DisableAttackCollision, 0.5f, false);
}

void AEnemyCharacterAi::DisableAttackCollision()
{
	attackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyCharacterAi::EnemyTakeDamage(const float damage)
{
	currentHp -= damage;
	AShadow_of_the_DesertGameState* gameState = Cast<AShadow_of_the_DesertGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		gameState->SetDamage(damage);
		if (currentHp <= 0&&!isDead)
		{
			PlayDeadAnimation();
			gameState->KillEnemy(scorePoint);
			UnpossessAI();
			isDead = true;
			FTimerHandle delayTime;
			GetWorld()->GetTimerManager().SetTimer(delayTime, this, &AEnemyCharacterAi::EnemyDespawn, 5.0f, false);
		}
	}
}

void AEnemyCharacterAi::EnemyDespawn()
{
	Destroy();
}

void AEnemyCharacterAi::PlayAttackAnimation()
{
	if (attackAnim && GetMesh())
	{
//		FTimerHandle attackTimer;
//		GetWorld()->GetTimerManager().SetTimer(attackTimer, this, &AEnemyCharacterAi::ActivateAttackCollision, 0.5f, false);
		GetMesh()->PlayAnimation(attackAnim, false);
	}
}

void AEnemyCharacterAi::PlayDeadAnimation()
{
	if (attackAnim && GetMesh())
	{
		GetMesh()->PlayAnimation(deadAnim, false);
	}
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

float AEnemyCharacterAi::GetAtkPower()
{
//	UE_LOG(LogTemp, Warning, TEXT("%.2f"), attackPower);
	return attackPower;
}

void AEnemyCharacterAi::UnpossessAI()
{
	AAIController* aiCtl = Cast<AAIController>(GetController());

	if (aiCtl)
	{
		aiCtl->UnPossess();
	}
}