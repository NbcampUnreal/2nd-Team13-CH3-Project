#include "Bullet/ExplosiveBullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"


AExplosiveBullet::AExplosiveBullet()
{
    Speed = 200.0f;
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
        Super::BulletEffects();
    }
}
/*
void AExplosiveBullet::Explode(FVector Location)
{
    // 폭발 효과 (파티클, 사운드 등) 추가 가능

    TArray<AActor*> IgnoredActors; // 데미지를 무시할 액터 목록
    ACharacter* InstigatorCharacter = Cast <ACharacter>(GetInstigator());
    if (InstigatorCharacter)
    {
        UE_LOG(LogTemp, Warning, TEXT("Step:1 Character"));
        IgnoredActors.Add(InstigatorCharacter);
    }

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("Step:2 PlayerController"));
        ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerController->GetCharacter());
        if (PlayerCharacter)
        {
            UE_LOG(LogTemp, Warning, TEXT("Step:3 PlayerController->Character"));
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

    if (GetInstigator())
    {
        UE_LOG(LogTemp, Warning, TEXT("Instigator is valid."));
        if (GetInstigator()->GetController())
        {
            UE_LOG(LogTemp, Warning, TEXT("Controller is valid."));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Controller is null!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Instigator is null in Explode!!"));
    }
}
*/

void AExplosiveBullet::Explode(FVector Location)
{

    TArray<AActor*> IgnoredActors;

    // 플레이어 캐릭터 추가 (이중 체크)
    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
    {
        IgnoredActors.AddUnique(PlayerPawn);
        UE_LOG(LogTemp, Warning, TEXT("Added Player: %s"), *GetNameSafe(PlayerPawn)); // [[1]](#__1)
    }

    // Instigator 유효성 검사 개선
    AController* DamageInstigator = nullptr;
    if (AActor* ValidInstigator = GetInstigator())
    {
        IgnoredActors.AddUnique(ValidInstigator);
        DamageInstigator = ValidInstigator->GetInstigatorController();
    }

    UE_LOG(LogTemp, Warning, TEXT("Ignored Actors Count: %d"), IgnoredActors.Num());

    UGameplayStatics::ApplyRadialDamage(
        GetWorld(),
        BulletDamage,
        Location,
        ExplosionRadius,
        nullptr,
        IgnoredActors,
        this, // Damage Causer
        DamageInstigator // Controller
    );
    UE_LOG(LogTemp, Warning, TEXT("Damage Applied: %.2f"), BulletDamage); // [[4]](#__4)
}
