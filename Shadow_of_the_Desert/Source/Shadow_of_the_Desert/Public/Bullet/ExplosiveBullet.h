#pragma once

#include "CoreMinimal.h"
#include "Bullet/BulletBase.h"
#include "ExplosiveBullet.generated.h"


UCLASS()
class SHADOW_OF_THE_DESERT_API AExplosiveBullet : public ABulletBase
{
	GENERATED_BODY()
public:
	AExplosiveBullet();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	float ExplosionRadius; // Æø¹ß ¹üÀ§

	UFUNCTION()
	void Explode(FVector Location);

	virtual void OnHit(AActor* OtherActor) override;
};
