#pragma once

#include "CoreMinimal.h"
#include "Bullet/BulletBase.h"
#include "PiercingBullet.generated.h"

UCLASS()
class SHADOW_OF_THE_DESERT_API APiercingBullet : public ABulletBase
{
	GENERATED_BODY()
public:
	APiercingBullet();
protected:
	virtual void OnHit(AActor* OtherActor);
};
