#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Sniper.generated.h"


UCLASS()
class SHADOW_OF_THE_DESERT_API ASniper : public AWeaponBase
{
	GENERATED_BODY()
public:
	ASniper();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload()override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void CompleteReload()override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Attack()override;
};
