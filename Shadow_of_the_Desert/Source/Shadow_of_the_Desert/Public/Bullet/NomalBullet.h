#pragma once

#include "CoreMinimal.h"
#include "Bullet/BulletBase.h"
#include "NomalBullet.generated.h"


UCLASS()
class SHADOW_OF_THE_DESERT_API ANomalBullet : public ABulletBase
{
	GENERATED_BODY()
public:
	ANomalBullet();


	virtual void OnHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;
};
