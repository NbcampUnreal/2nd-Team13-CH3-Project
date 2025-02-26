#include "Bullet/BulletBase.h"
#include "Components/StaticMeshComponent.h"
//#include "EnemyCharater.h"


ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMesh;

	// 콜리전 설정
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(BulletMesh);
	ExplosionCollision->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnHit);

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

void ABulletBase::OnHit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
}