#include "Bullet/BulletBase.h"
#include "Components/StaticMeshComponent.h"
//#include "EnemyCharater.h"


ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMesh;
	// RootComponent를 별도로 생성하는 것이 더 안전합니다.
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	BulletMesh->SetupAttachment(Root); // 메시를 루트에 부착

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

	// 매시 방향 설정
	if (!MovementDirection.IsNearlyZero())
	{
		const FRotator NewRotation = FRotationMatrix::MakeFromX(MovementDirection).Rotator();
		SetActorRotation(NewRotation);
	}
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