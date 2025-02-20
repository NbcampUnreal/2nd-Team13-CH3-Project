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
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float AttackDamage;
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    int32 CurrentAmmo;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    int32 MaxAmmo;//�ִ� ���� źâ ��
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    bool bIsReloading; // ���ε� ������ ����
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float AttackRate; // �߻� �ӵ� (�� ����)
    float LastAttackTime;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ABulletBase> BulletClass; // �߻��� �Ѿ��� Ŭ���� Ÿ��
    
	virtual void Reload();
    virtual void CompleteReload();
    virtual void Attack();
protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* WeaponMesh;
};