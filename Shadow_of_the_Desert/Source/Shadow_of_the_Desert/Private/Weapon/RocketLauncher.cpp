#include "Weapon/RocketLauncher.h"

ARocketLauncher::ARocketLauncher()
{
	//����
	AttackDamage = 100.0f;
	CurrentAmmo = 1;
	MaxAmmo = 1;
	bIsReloading = false;
	AttackRate = 2.0f;
	ReloadTime = 3.0f;
	SpreadAngle = 0.01f;

	//������ �� ������
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
}

void ARocketLauncher::Attack()
{
	Super::Attack();
}