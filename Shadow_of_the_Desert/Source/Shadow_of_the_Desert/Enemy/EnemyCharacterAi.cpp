// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterAi.h"
#include "EnemyAIController.h"
#include "CustomHUD.h"
#include "../Shadow_of_the_DesertCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.h"
#include "Components/TextBlock.h"
#include "Camera/PlayerCameraManager.h"

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

void AEnemyCharacterAi::BeginPlay()
{
	Super::BeginPlay();

	if (!DamageTextWidgetClass)
	{
		// 블루프린트 클래스를 런타임에 로드
		DamageTextWidgetClass = LoadClass<UDamageTextWidget>(nullptr, TEXT("/Game/UI/Widgets/WBP_DamageText.WBP_DamageText_C"));
		if (!DamageTextWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load WBP_DamageText!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully loaded WBP_DamageText!"));
		}
	}
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

		// 적 머리 위에 데미지 숫자 표시
		ShowDamageText(static_cast<int32>(damage));

		// 히트마커 표시 (적이 맞았을 때)
		APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerController)
		{
			ACustomHUD* HUD = Cast<ACustomHUD>(PlayerController->GetHUD());
			if (HUD)
			{
				HUD->ShowHitmarker(); // 히트마커 활성화
			}
		}

		if (currentHp <= 0&&!isDead)
		{
			// 킬마커 표시 (적이 죽었을 때)
			if (PlayerController)
			{
				ACustomHUD* HUD = Cast<ACustomHUD>(PlayerController->GetHUD());
				if (HUD)
				{
					HUD->ShowKillmarker(); // 킬마커 활성화
				}
			}

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

void AEnemyCharacterAi::ShowDamageText(int32 Damage)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is NULL!"));
		return;
	}

	// WBP_DamageText 블루프린트 UI 위젯 생성
	UUserWidget* DamageWidget = CreateWidget<UUserWidget>(PC, LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/Widgets/WBP_DamageText.WBP_DamageText_C")));
	if (!DamageWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create WBP_DamageText!"));
		return;
	}

	// 위젯 내부에서 "DamageText"라는 이름의 TextBlock 찾기
	UTextBlock* DamageText = Cast<UTextBlock>(DamageWidget->GetWidgetFromName(TEXT("DamageText")));
	if (DamageText)
	{
		DamageText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Damage)));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find DamageText in WBP_DamageText!"));
	}

	// 위젯을 화면에 추가
	DamageWidget->AddToViewport();
}

