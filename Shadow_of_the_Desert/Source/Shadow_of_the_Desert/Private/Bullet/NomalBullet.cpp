#include "Bullet/NomalBullet.h"

ANomalBullet::ANomalBullet()
{
	Speed = 1000.0f;
	BulletDamage = 0.0f;
}

//���̳� ������ �� �ϳ��� �ε����� �����
void ANomalBullet::OnHit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	/*
	if (OtherActor && OtherActor->IsA(AEnemyCharacter::StaticClass()))
	{
		AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(OtherActor);
		if (HitEnemy)
		{
			HitEnemy->TakeDamage(BulletDamage, FDamageEvent(), nullptr, this);
		}
		Destroy();
	}	*/
}