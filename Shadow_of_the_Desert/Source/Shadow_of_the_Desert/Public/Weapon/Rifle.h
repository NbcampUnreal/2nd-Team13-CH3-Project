#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Rifle.generated.h"


UCLASS()
class SHADOW_OF_THE_DESERT_API ARifle : public AWeaponBase
{
	GENERATED_BODY()
public:
	ARifle();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload()override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void CompleteReload()override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Attack()override;

	virtual void UpgradeAttackDamage()override;
	virtual void UpgradeAttackRate()override;
	virtual void UpgradeMaxAmmo()override;
	virtual void UpgradeReloadTime()override;
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AmmoMesh;
};
