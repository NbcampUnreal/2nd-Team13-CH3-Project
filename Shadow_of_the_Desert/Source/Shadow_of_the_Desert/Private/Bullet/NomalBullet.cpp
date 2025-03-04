#include "Bullet/NomalBullet.h"

ANomalBullet::ANomalBullet()
{
	Speed = 1000.0f;
	BulletDamage = 0.0f;
}

//적이나 구조물 중 하나에 부딪히면 사라짐
void ANomalBullet::OnHit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnHit called with %s"), *OtherActor->GetName());
		if (OtherActor && OtherActor->IsA(AEnemyCharacterAi::StaticClass()))
		{
			AEnemyCharacterAi* HitEnemy = Cast<AEnemyCharacterAi>(OtherActor);
			if (HitEnemy)
			{
				HitEnemy->EnemyTakeDamage(BulletDamage);
			}
			Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Damage Applied: %.2f"), BulletDamage); // [[4]](#__4)
		}
		else
		{
			Destroy();
		}
	}
}