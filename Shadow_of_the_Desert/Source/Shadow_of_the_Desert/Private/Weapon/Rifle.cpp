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
	UE_LOG(LogTemp, Warning, TEXT("Reloading complete."));
	bIsReloading = false;
}

void ARifle::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Plz."));

	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentAmmo > 0 && !bIsReloading && (CurrentTime - LastAttackTime >= AttackRate))
	{

		FRotator CameraRotation;
		FVector WeaponLocation = GetActorLocation() + CameraRotation.Vector() * 100.0f;
		//³­¼ö(ÃÑ¾Ë Æ¢´Â°Å)
		float RandomOffsetX = FMath::FRandRange(-0.1f, 0.1f);
		float RandomOffsetY = FMath::FRandRange(-0.1f, 0.1f);
		FVector Direction = (CameraRotation.Vector() + FVector(RandomOffsetX, RandomOffsetY, 0)).GetSafeNormal();

		ABulletBase* Bullet = GetWorld()->SpawnActor<ABulletBase>(BulletClass, WeaponLocation, FRotator::ZeroRotator);
		if (Bullet)
		{
			Bullet->Initialize(Direction, AttackDamage);
		}   
		UE_LOG(LogTemp, Warning, TEXT("Good! Ammo %d"), CurrentAmmo);
		CurrentAmmo--;
		LastAttackTime = CurrentTime;
	}
	if(CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reloading Plz."));
	}
}