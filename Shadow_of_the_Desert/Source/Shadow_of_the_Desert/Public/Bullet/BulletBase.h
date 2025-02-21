#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere)
	float Speed;

	FVector MovementDirection;
	float BulletDamage;

	virtual void OnHit(AActor* OtherActor);
};
