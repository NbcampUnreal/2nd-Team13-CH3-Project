#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet/BulletBase.h"
#include "WeaponBase.generated.h"

UCLASS()
class SHADOW_OF_THE_DESERT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
    AWeaponBase();
    
	virtual void Reload();
    virtual void CompleteReload();
    virtual void Attack();
    virtual void UpgradeAttackDamage();
    virtual void UpgradeAttackRate();
    virtual void UpgradeMaxAmmo();
    virtual void UpgradeReloadTime();


protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    float AttackDamage;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    int32 CurrentAmmo;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    int32 MaxAmmo;//최대 장전 탄창 수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    bool bIsReloading; // 리로드 중인지 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    float ReloadTime; // 재장전 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    float AttackRate; // 연사 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    float SpreadAngle;// 탄퍼짐 각도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    float LastAttackTime;//마지막 발사 시각
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ABulletBase> BulletClass; // 발사할 총알의 클래스 타입

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Upgrade")
    float DamageIncreaseRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Upgrade")
    float AttackRateIncreaseRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Upgrade")
    int32 AmmoIncreaseRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Upgrade")
    float ReloadTimeIncreaseRate;
};