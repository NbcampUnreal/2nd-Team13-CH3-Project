#include "Weapon/RocketLauncher.h"

ARocketLauncher::ARocketLauncher()
{
	RocketLauncherBulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RocketLauncherBulletMesh"));
	RocketLauncherBulletMesh->SetupAttachment(WeaponMesh);

	//스탯
	AttackDamage = 100.0f;
	CurrentAmmo = 1;
	MaxAmmo = 1;
	bIsReloading = false;
	AttackRate = 2.0f;
	ReloadTime = 3.0f;
	SpreadAngle = 0.01f;

	//레벨업 시 증가량
	DamageIncreaseRate = 20.0f;
	AttackRateIncreaseRate = 0.95f;
	AmmoIncreaseRate = 1;
	ReloadTimeIncreaseRate = 0.9f;
}

void ARocketLauncher::Reload()
{
	Super::Reload();
}

void ARocketLauncher::CompleteReload()
{
	Super::CompleteReload();
	if (!RocketLauncherBulletMesh->IsVisible())
	{
		RocketLauncherBulletMesh->SetVisibility(true);
	}
}

void ARocketLauncher::Attack()
{
	Super::Attack();
	if (CurrentAmmo <= 0)
	{
		RocketLauncherBulletMesh->SetVisibility(false);
	}
}