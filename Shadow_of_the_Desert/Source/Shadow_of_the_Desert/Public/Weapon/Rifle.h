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
	virtual void Reload()override;
	virtual void CompleteReload()override;
	virtual void Attack()override;
};
