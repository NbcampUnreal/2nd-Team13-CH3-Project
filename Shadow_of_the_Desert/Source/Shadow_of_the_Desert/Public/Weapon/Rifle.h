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
	virtual void CompleteReload()override;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Attack()override;
};
