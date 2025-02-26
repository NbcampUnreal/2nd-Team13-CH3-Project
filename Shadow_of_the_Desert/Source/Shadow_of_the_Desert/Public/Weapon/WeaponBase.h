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
    int32 MaxAmmo;//�ִ� ���� źâ ��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    bool bIsReloading; // ���ε� ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    float ReloadTime; // ������ �ð�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    float AttackRate; // ���� �ӵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    float SpreadAngle;// ź���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Stat")
    float LastAttackTime;//������ �߻� �ð�
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ABulletBase> BulletClass; // �߻��� �Ѿ��� Ŭ���� Ÿ��

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Upgrade")
    float DamageIncreaseRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Upgrade")
    float AttackRateIncreaseRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Upgrade")
    int32 AmmoIncreaseRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Upgrade")
    float ReloadTimeIncreaseRate;
};