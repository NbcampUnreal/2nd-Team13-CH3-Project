#include "Weapon/Rifle.h"

ARifle::ARifle()
{
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	AmmoMesh->SetupAttachment(WeaponMesh);

	//����
	AttackDamage = 5.0f;
	CurrentAmmo = 30;
	MaxAmmo = 30;
	bIsReloading = false;
	AttackRate = 0.2f;
	ReloadTime = 1.5f;
	SpreadAngle = 0.01f;

	//������ �� ������
	DamageIncreaseRate = 1.0f;
	AttackRateIncreaseRate = 0.9f;
	AmmoIncreaseRate = 5;
	ReloadTimeIncreaseRate = 0.85f;
}

void ARifle::Reload()
{
	Super::Reload();
}

void ARifle::CompleteReload()
{
	Super::CompleteReload();
}

void ARifle::Attack()
{
	Super::Attack();
}

void ARifle::UpgradeAttackDamage()
{
	Super::UpgradeAttackDamage();
}

void ARifle::UpgradeAttackRate()
{
	Super::UpgradeAttackRate();
}

void ARifle::UpgradeMaxAmmo()
{
	Super::UpgradeMaxAmmo();
}

void ARifle::UpgradeReloadTime()
{
	Super::UpgradeReloadTime();
}