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
	UE_LOG(LogTemp, Warning, TEXT("OnHit called with %s"), *OtherActor->GetName());
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
