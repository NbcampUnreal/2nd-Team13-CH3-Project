#include "Weapon/Rifle.h"
#include "Engine/World.h"

ARifle::ARifle()
{
	AttackDamage = 5.0f;
	CurrentAmmo = 30;
	MaxAmmo = 30;
	bIsReloading = false;
	AttackRate = 0.1f;
}


void ARifle::Reload()
{
	if (CurrentAmmo < MaxAmmo)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARifle::CompleteReload, 1.5f, false);

				bIsReloading = true;
			});
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ammo is already full."));
	}
}

void ARifle::CompleteReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Reload Complete! CurrentAmmo: %d, bIsReloading: %s, LastAttackTime: %f"),
		CurrentAmmo, bIsReloading ? TEXT("true") : TEXT("false"),
		LastAttackTime);
}

void ARifle::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("CurrentAmmo: %d, bIsReloading: %s, LastAttackTime: %f"),
		CurrentAmmo, bIsReloading ? TEXT("true") : TEXT("false"),
		LastAttackTime);
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("rerererere"))
	}
	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentAmmo > 0 && !bIsReloading && (CurrentTime - LastAttackTime >= AttackRate))
	{
		UE_LOG(LogTemp, Warning, TEXT("Shot"));

		FRotator CameraRotation;
		FVector CameraLocation;


		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
			FVector WeaponLocation = GetActorLocation() + CameraRotation.Vector() * 100.0f;//ÃÑ¾Ë ¹ß»ç À§Ä¡
			//³­¼ö(ÃÑ¾Ë Æ¢´Â°Å)
			float RandomOffsetX = FMath::FRandRange(-0.1f, 0.1f);
			float RandomOffsetY = FMath::FRandRange(-0.1f, 0.1f);
			FVector Direction = (CameraRotation.Vector() + FVector(RandomOffsetX, RandomOffsetY, 0)).GetSafeNormal();

			ABulletBase* Bullet = GetWorld()->SpawnActor<ABulletBase>(BulletClass, WeaponLocation, FRotator::ZeroRotator);
			if (Bullet)
			{
				Bullet->Initialize(Direction, AttackDamage);
			}
			CurrentAmmo--;
			UE_LOG(LogTemp, Warning, TEXT("END CurrentAmmo: %d, bIsReloading: %s, LastAttackTime: %f"),
				CurrentAmmo, bIsReloading ? TEXT("true") : TEXT("false"),
				LastAttackTime);
			LastAttackTime = CurrentTime;
		}
	}
	if(CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloading Plz."));
	}
}