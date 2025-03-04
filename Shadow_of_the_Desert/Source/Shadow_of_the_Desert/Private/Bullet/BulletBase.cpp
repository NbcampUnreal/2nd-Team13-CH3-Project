#include "Bullet/BulletBase.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"


ABulletBase::ABulletBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	RootComponent = BulletMesh;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	BulletMesh->SetupAttachment(Root);

	// 콜리전 설정
	HitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	HitCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HitCollision->SetupAttachment(BulletMesh);
	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::OnHit);

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

void ABulletBase::Initialize(FVector Direction, float Damage, APawn* InstigatorPawn)
{
	MovementDirection = Direction;
	BulletDamage = Damage;
	SetInstigator(InstigatorPawn);

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

void ABulletBase::BulletEffects()
{
	UParticleSystemComponent* Particle = nullptr;

	if (HitParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
	}
	if (HitSound)
	{
		UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			HitSound,
			GetActorLocation()
		);
	}
}