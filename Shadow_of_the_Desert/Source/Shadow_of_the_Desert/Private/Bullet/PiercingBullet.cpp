#include "Bullet/PiercingBullet.h"

APiercingBullet::APiercingBullet()
{
	Speed = 9000.0f;
	BulletDamage = 0.0f;
}

void APiercingBullet::OnHit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor == GetOwner())
	{
		return;
	}    

	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHit called with %s"), *OtherActor->GetName());
		if (OtherActor && OtherActor->IsA(AEnemyCharacterAi::StaticClass()))
		{
			if (AEnemyCharacterAi* HitEnemy = Cast<AEnemyCharacterAi>(OtherActor))
			{
				if (HitEnemy)
				{
					HitEnemy->EnemyTakeDamage(BulletDamage);
				}
				UE_LOG(LogTemp, Warning, TEXT("Damage Applied: %.2f"), BulletDamage); // [[4]](#__4)
			}
		}
		else
		{
			Destroy();
		}
	}
}
