#include "Weapon/WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

AWeaponBase::AWeaponBase()
{
	LastAttackTime = 0.0f;
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	MuzzleMesh = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleMesh->SetupAttachment(WeaponMesh);
	WeaponMesh->SetSimulatePhysics(false);
	//초기화
	AttackDamage = 0.0f;
	CurrentAmmo = 0;
	MaxAmmo = 0;
	bIsReloading = false;
	AttackRate = 0.0f;
	ReloadTime = 0.0f;
	SpreadAngle = 0.0f;

	DamageIncreaseRate = 0;
}

void AWeaponBase::Reload()
{
	if (bIsReloading) return;

	if (CurrentAmmo < MaxAmmo)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWeaponBase::CompleteReload, ReloadTime, false);

				bIsReloading = true;
			});
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ammo is already full."));
	}
}

void AWeaponBase::CompleteReload()
{
	CurrentAmmo = MaxAmmo;
	bIsReloading = false;
	UE_LOG(LogTemp, Warning, TEXT("Reload Complete! CurrentAmmo: %d, bIsReloading: %s, LastAttackTime: %f"),
		CurrentAmmo, bIsReloading ? TEXT("true") : TEXT("false"),
		LastAttackTime);
}

void AWeaponBase::CancelReload()
{
	if (bIsReloading)
	{
		// 리로드 중인 경우 타이머를 취소
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle); // TimerHandle은 리로드 타이머 핸들입니다.

		bIsReloading = false; // 리로드 상태 변경
		UE_LOG(LogTemp, Warning, TEXT("Reload canceled."));
	}
}

void AWeaponBase::Attack()
{
	if (bIsReloading)
	{
		UE_LOG(LogTemp, Warning, TEXT("rerererere"))
		return;
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentAmmo > 0 && (CurrentTime - LastAttackTime >= AttackRate))
	{
		FRotator CameraRotation;
		FVector CameraLocation;

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			FVector WeaponLocation = MuzzleMesh->GetComponentLocation(); // 총구 위치
			//탄퍼짐
			FVector RandomOffset = FMath::VRand() * SpreadAngle;
			FVector Direction = (CameraRotation.Vector() + RandomOffset).GetSafeNormal();

			APawn* CharacterInstigator = Cast<APawn>(GetOwner()); // 총알을 발사하는 캐릭터
			UE_LOG(LogTemp, Warning, TEXT("Current Actor: %s"), *GetName());

			ABulletBase* Bullet = GetWorld()->SpawnActor<ABulletBase>(BulletClass, WeaponLocation, FRotator::ZeroRotator);
			if (Bullet)
			{
				Bullet->Initialize(Direction, AttackDamage, CharacterInstigator);
			}
			CurrentAmmo--;
			UE_LOG(LogTemp, Warning, TEXT("END CurrentAmmo: %d, bIsReloading: %s, LastAttackTime: %f"),
				CurrentAmmo, bIsReloading ? TEXT("true") : TEXT("false"),
				LastAttackTime);
			LastAttackTime = CurrentTime;
		}
	}
}

void AWeaponBase::UpgradeAttackDamage()
{
	AttackDamage += DamageIncreaseRate;
}

void AWeaponBase::UpgradeAttackRate()
{
	AttackRate *= AttackRateIncreaseRate;

	AttackRate = FMath::RoundToFloat(AttackRate * 100.0f) / 100.0f;
}

void AWeaponBase::UpgradeMaxAmmo()
{
	MaxAmmo += AmmoIncreaseRate;
}

void AWeaponBase::UpgradeReloadTime()
{
	ReloadTime *= ReloadTimeIncreaseRate;

	ReloadTime = FMath::RoundToFloat(ReloadTime * 100.0f) / 100.0f;
}