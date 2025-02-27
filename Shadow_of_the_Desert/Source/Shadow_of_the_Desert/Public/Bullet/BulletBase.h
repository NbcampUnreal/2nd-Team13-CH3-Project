#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "BulletBase.generated.h"

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

	void Initialize(FVector Direction, float Damage);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void OnHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere)
	float Speed;

	FVector MovementDirection;
	float BulletDamage;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* ExplosionCollision;

};
