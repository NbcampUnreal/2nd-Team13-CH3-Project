#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "RocketLauncher.generated.h"

UCLASS()
class SHADOW_OF_THE_DESERT_API ARocketLauncher : public AWeaponBase
{
	GENERATED_BODY()
public:
	ARocketLauncher();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload()override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void CompleteReload()override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Attack()override;
};
