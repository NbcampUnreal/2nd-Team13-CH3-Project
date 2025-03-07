#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "Shadow_of_the_Desert/Enemy/EnemyCharacterAi.h"
#include "BulletBase.generated.h"

class AEnemyCharacterAi;

UCLASS()

class SHADOW_OF_THE_DESERT_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletBase();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

	void Initialize(FVector Direction, float Damage, APawn* InstigatorPawn);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	
	virtual void BulletEffects();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* HitCollision;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Effects")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Effects")
	UParticleSystem* HitParticle;

	UPROPERTY(VisibleAnywhere)
	float Speed;

	FVector MovementDirection;
	float BulletDamage;
};
