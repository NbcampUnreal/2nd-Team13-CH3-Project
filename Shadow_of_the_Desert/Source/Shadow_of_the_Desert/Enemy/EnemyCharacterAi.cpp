// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacterAi.h"
#include "CustomHUD.h"
#include "../Shadow_of_the_DesertCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "DamageTextWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
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
		DamageTextWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/Widgets/WBP_DamageText.WBP_DamageText_C"));
	}

	USkeletalMeshComponent* meshComp = GetMesh();
	if (meshComp)
	{
		originMaterial = meshComp->CreateAndSetMaterialInstanceDynamic(0);

		hitMaterial = UMaterialInstanceDynamic::Create(originMaterial->GetMaterial(), this);
		if (hitMaterial)
		{
			hitMaterial->SetVectorParameterValue("BaseColor", FLinearColor::Red);
		}
	}
}

void AEnemyCharacterAi::EnemyAttack()
{
	FTimerHandle attackTimer;
	GetWorld()->GetTimerManager().SetTimer(attackTimer, [this]()
		{
			attackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}, attackSpeed, false);

	PlayAttackAnimation();
	DisableAttackCollision();
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
		if (playerChr && aiCtl)
		{
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
	AShadow_of_the_DesertGameState* gameState = Cast<AShadow_of_the_DesertGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		if (!isDead)
		{
			currentHp -= damage;
			gameState->SetDamage(damage);
			ShowDamageText(static_cast<int32>(damage));

			APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			if (PlayerController)
			{
				ACustomHUD* HUD = Cast<ACustomHUD>(PlayerController->GetHUD());
				if (HUD)
				{
					HUD->ShowHitmarker();
				}
			}

			if (currentHp <= 0)
			{
				if (PlayerController)
				{
					ACustomHUD* HUD = Cast<ACustomHUD>(PlayerController->GetHUD());
					if (HUD)
					{
						HUD->ShowKillmarker();
					}
				}
				PlayDeadAnimation();
				gameState->KillEnemy(scorePoint);
				isDead = true;
				FTimerHandle delayTime;
				GetWorld()->GetTimerManager().SetTimer(delayTime, this, &AEnemyCharacterAi::EnemyDespawn, 5.0f, false);
				UnpossessAI();
			}
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

float AEnemyCharacterAi::GetAtkPower()
{
	return attackPower;
}

void AEnemyCharacterAi::UnpossessAI()
{
	AAIController* aiCtl = Cast<AAIController>(GetController());
	if (aiCtl)
	{
		aiCtl->Destroy();
		aiCtl->UnPossess();
	}
}

void AEnemyCharacterAi::SetHitMaterial()
{
	if (GetMesh() && hitMaterial)
	{
		FTimerHandle resetTimer;
		GetMesh()->SetMaterial(0, hitMaterial);
		GetWorld()->GetTimerManager().SetTimer(resetTimer, this, &AEnemyCharacterAi::ResetMaterial, 0.3f, false);
	}
}

void AEnemyCharacterAi::ResetMaterial()
{
	if (GetMesh() && originMaterial)
	{
		GetMesh()->SetMaterial(0, originMaterial);
	}
}

void AEnemyCharacterAi::ShowDamageText(int32 Damage)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC || !DamageTextWidgetClass) return;

	UUserWidget* DamageWidget = CreateWidget<UUserWidget>(PC, DamageTextWidgetClass);
	if (!DamageWidget) return;

	DamageWidget->AddToViewport();

	UTextBlock* DamageText = Cast<UTextBlock>(DamageWidget->GetWidgetFromName(TEXT("DamageText")));
	if (DamageText)
	{
		DamageText->SetText(FText::AsNumber(Damage));

		FLinearColor Color;
		if (Damage < 100)
		{
			Color = FLinearColor(1.f, 1.f, 0.6f);
		}
		else if (Damage < 1000)
		{
			Color = FLinearColor(1.f, 0.5f, 0.f);
		}
		else
		{
			Color = FLinearColor(1.f, 0.2f, 0.2f);
		}

		DamageText->SetColorAndOpacity(FSlateColor(Color));
	}

	FVector WorldLocation = GetActorLocation() + FVector(0, 0, 120);
	FVector2D ScreenPosition;
	if (PC->ProjectWorldLocationToScreen(WorldLocation, ScreenPosition))
	{
		DamageWidget->SetPositionInViewport(ScreenPosition, true);
	}

	FTimerHandle RemoveTimer;
	GetWorld()->GetTimerManager().SetTimer(RemoveTimer, [DamageWidget]()
		{
			DamageWidget->RemoveFromParent();
		}, 1.5f, false);
}
