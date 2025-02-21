#include "Weapon/WeaponBase.h"
#include "Components/StaticMeshComponent.h"

AWeaponBase::AWeaponBase()
{
	LastAttackTime = 0.0f;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

void AWeaponBase::Reload()
{
}

void AWeaponBase::CompleteReload()
{
}

void AWeaponBase::Attack()
{
}