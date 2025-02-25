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


	virtual void OnHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	)override;
};
