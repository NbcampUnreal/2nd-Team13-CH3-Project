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
    int32 MaxAmmo;//최대 장전 탄창 수
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    bool bIsReloading; // 리로드 중인지 여부
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float AttackRate; // 발사 속도 (초 단위)
    float LastAttackTime;
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<ABulletBase> BulletClass; // 발사할 총알의 클래스 타입
    
	virtual void Reload();
    virtual void CompleteReload();
    virtual void Attack();
protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* WeaponMesh;
};