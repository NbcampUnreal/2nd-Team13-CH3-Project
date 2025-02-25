#include "Bullet/ExplosiveBullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


AExplosiveBullet::AExplosiveBullet()
{
    ExplosionRadius = 500.0f; // 폭발 범위
    BulletDamage = 0;   // 폭발 데미지
}

void AExplosiveBullet::BeginPlay()
{
    Super::BeginPlay();


    BulletMesh->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveBullet::OnHit);
}

void AExplosiveBullet::OnHit(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (OtherActor == GetOwner())
    {
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("OnHit called with %s"), *OtherActor->GetName());
    if (OtherActor && OtherActor != this)
    {
        Explode(GetActorLocation());
        Destroy();
    }
}

void AExplosiveBullet::Explode(FVector Location)
{
    // 폭발 효과 (파티클, 사운드 등) 추가 가능

    TArray<AActor*> IgnoredActors; // 데미지를 무시할 액터 목록
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerController->GetCharacter());
        if (PlayerCharacter)
        {
            IgnoredActors.Add(Cast<AActor>(PlayerCharacter));
        }
    }

    if (GetInstigator() && GetInstigator()->GetController())
    {
        // 폭발 데미지 로직
        UGameplayStatics::ApplyRadialDamage(
            GetWorld(),
            BulletDamage,
            GetActorLocation(),
            ExplosionRadius,
            nullptr,
            IgnoredActors,
            this,
            GetInstigator()->GetController() 
        );
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Instigator or Controller is null!"));
    }
}