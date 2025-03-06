#include "Bullet/ExplosiveBullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"



AExplosiveBullet::AExplosiveBullet()
{
    Speed = 1000.0f;
    ExplosionRadius = 800.0f; // 폭발 범위
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


/*void AExplosiveBullet::Explode(FVector Location)
{

    TArray<AActor*> IgnoredActors;

    // 플레이어 캐릭터 추가
    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
    {
        IgnoredActors.AddUnique(PlayerPawn);
        UE_LOG(LogTemp, Warning, TEXT("Added Player: %s"), *GetNameSafe(PlayerPawn)); // [[1]](#__1)
    }

    AController* DamageInstigator = nullptr;
    if (AActor* ValidInstigator = GetInstigator())
    {
        IgnoredActors.AddUnique(ValidInstigator);
        DamageInstigator = ValidInstigator->GetInstigatorController();
    }

    UE_LOG(LogTemp, Warning, TEXT("Ignored Actors Count: %d"), IgnoredActors.Num());
    TArray<AActor*> AEnemyActors;
    TArray<AActor*> AllEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacterAi::StaticClass(), AllEnemies);
    for (AActor* Actor : AllEnemies)
    {
        if (Actor && FVector::Dist(Actor->GetActorLocation(), Location) <= ExplosionRadius)
        {
            AEnemyActors.Add(Actor); // 범위 내의 적 추가
        }
    }
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
    DrawDebugSphere(GetWorld(), Location, ExplosionRadius, 12, FColor::Red, false, 3.0f);  // 
    for (AActor* Actor : AEnemyActors)
    {
        if (AEnemyCharacterAi* Enemy = Cast<AEnemyCharacterAi>(Actor))
        {
            Enemy->EnemyTakeDamage(BulletDamage);
            UE_LOG(LogTemp, Warning, TEXT("Enemy %s took damage: %.2f"), *Enemy->GetName(), BulletDamage);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Actor %s is not an enemy"), *Actor->GetName());
        }
    }
}*/
/*void AExplosiveBullet::Explode(FVector Location)
{
    TArray<AActor*> IgnoredActors;

    // 플레이어 캐릭터 추가
    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
    {
        IgnoredActors.AddUnique(PlayerPawn);
        UE_LOG(LogTemp, Warning, TEXT("Added Player: %s"), *GetNameSafe(PlayerPawn));
    }

    AController* DamageInstigator = nullptr;
    if (AActor* ValidInstigator = GetInstigator())
    {
        IgnoredActors.AddUnique(ValidInstigator);
        DamageInstigator = ValidInstigator->GetInstigatorController();
    }

    // 모든 적을 가져와서 범위 내의 적을 찾아서 데미지 적용
    TArray<AActor*> AllEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacterAi::StaticClass(), AllEnemies);
    for (AActor* Actor : AllEnemies)
    {
        if (Actor && FVector::Dist(Actor->GetActorLocation(), Location) <= ExplosionRadius)
        {
            // 적에게 데미지 적용
            if (AEnemyCharacterAi* Enemy = Cast<AEnemyCharacterAi>(Actor))
            {
                Enemy->EnemyTakeDamage(BulletDamage);
                UE_LOG(LogTemp, Warning, TEXT("Enemy %s took damage: %.2f"), *Enemy->GetName(), BulletDamage);
            }
        }
    }

    // 원형 피해 적용 (적은 제외하고)
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

    // 디버그 로그
    UE_LOG(LogTemp, Warning, TEXT("Damage Applied: %.2f"), BulletDamage);
    DrawDebugSphere(GetWorld(), Location, ExplosionRadius, 12, FColor::Red, false, 3.0f);
}*/


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

    // 1. 무시할 액터 설정 (플레이어와 발사자)
    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
    {
        IgnoredActors.AddUnique(PlayerPawn);
    }

    if (AActor* InstigatorActor = GetInstigator())
    {
        IgnoredActors.AddUnique(InstigatorActor);
    }

    // 2. 범위 내 모든 AEnemyCharacterAi 찾기
    TArray<AActor*> AllEnemies;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AEnemyCharacterAi::StaticClass(),
        AllEnemies
    );

    // 3. 범위 내 적에게 직접 EnemyTakeDamage 호출
    for (AActor* Enemy : AllEnemies)
    {
        if (Enemy &&
            FVector::Dist(Enemy->GetActorLocation(), Location) <= ExplosionRadius)
        {
            if (AEnemyCharacterAi* EnemyChar = Cast<AEnemyCharacterAi>(Enemy))
            {
                // 적 클래스의 EnemyTakeDamage 직접 호출
                EnemyChar->EnemyTakeDamage(BulletDamage);
                UE_LOG(LogTemp, Warning, TEXT("Enemy %s took damage"), *EnemyChar->GetName());
            }
        }
    }
    // 4. 디버그 및 이펙트 처리
    //DrawDebugSphere(GetWorld(), Location, ExplosionRadius, 12, FColor::Red, false, 3.0f);
    Destroy();
}