#include "Bullet/PiercingBullet.h"

APiercingBullet::APiercingBullet()
{
	Speed = 9000.0f;
	BulletDamage = 0.0f;
}

void APiercingBullet::OnHit(AActor* OtherActor)
{
	/*if (OtherActor && OtherActor->IsA(AEnemyCharacter::StaticClass()))
	{
		AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(OtherActor);
		if (HitEnemy)
		{
			HitEnemy->TakeDamage(BulletDamage, FDamageEvent(), nullptr, this);
		}
	}
	else
	{
		Destroy();
	}*/
}
