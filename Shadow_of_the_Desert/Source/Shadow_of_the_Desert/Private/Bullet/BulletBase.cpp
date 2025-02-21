#include "Bullet/BulletBase.h"
#include "Components/StaticMeshComponent.h"

ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMesh;

	Speed = 3000.0f;
	BulletDamage = 0.0f;
}

void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation() + MovementDirection * Speed * DeltaTime;
	SetActorLocation(NewLocation);

}

void ABulletBase::Initialize(FVector Direction, float Damage)
{
	MovementDirection = Direction;
	BulletDamage = Damage;
}

void ABulletBase::OnHit(AActor* OtherActor)
{

}
