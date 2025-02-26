#include "Weapon/Sniper.h"

ASniper::ASniper()
{
	//스탯
	AttackDamage = 50.0f;
	CurrentAmmo = 5;
	MaxAmmo = 5;
	bIsReloading = false;
	AttackRate = 1.2f;
	ReloadTime = 2.0f;
	SpreadAngle = 0.01f;

	//레벨업 시 증가량
	DamageIncreaseRate = 10.0f;
	AttackRateIncreaseRate = 0.8f;
	AmmoIncreaseRate = 3;
	ReloadTimeIncreaseRate = 0.85f;
}

void ASniper::Reload()
{
	Super::Reload();
}

void ASniper::CompleteReload()
{
	Super::CompleteReload();
}

void ASniper::Attack()
{
	Super::Attack();
}