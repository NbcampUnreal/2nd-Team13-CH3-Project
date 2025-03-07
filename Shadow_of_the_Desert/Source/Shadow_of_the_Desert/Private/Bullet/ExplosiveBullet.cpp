#include "Bullet/ExplosiveBullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"



AExplosiveBullet::AExplosiveBullet()
{
    Speed = 1000.0f;
    ExplosionRadius = 800.0f; // ���� ����
    BulletDamage = 0;   // ���� ������
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

    // �÷��̾� ĳ���� �߰�
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
            AEnemyActors.Add(Actor); // ���� ���� �� �߰�
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

    // �÷��̾� ĳ���� �߰�
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

    // ��� ���� �����ͼ� ���� ���� ���� ã�Ƽ� ������ ����
    TArray<AActor*> AllEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacterAi::StaticClass(), AllEnemies);
    for (AActor* Actor : AllEnemies)
    {
        if (Actor && FVector::Dist(Actor->GetActorLocation(), Location) <= ExplosionRadius)
        {
            // ������ ������ ����
            if (AEnemyCharacterAi* Enemy = Cast<AEnemyCharacterAi>(Actor))
            {
                Enemy->EnemyTakeDamage(BulletDamage);
                UE_LOG(LogTemp, Warning, TEXT("Enemy %s took damage: %.2f"), *Enemy->GetName(), BulletDamage);
            }
        }
    }

    // ���� ���� ���� (���� �����ϰ�)
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

    // ����� �α�
    UE_LOG(LogTemp, Warning, TEXT("Damage Applied: %.2f"), BulletDamage);
    DrawDebugSphere(GetWorld(), Location, ExplosionRadius, 12, FColor::Red, false, 3.0f);
}*/


/*
void AExplosiveBullet::Explode(FVector Location)
{
    // ���� ȿ�� (��ƼŬ, ���� ��) �߰� ����

    TArray<AActor*> IgnoredActors; // �������� ������ ���� ���
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
        // ���� ������ ����
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

    // 1. ������ ���� ���� (�÷��̾�� �߻���)
    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
    {
        IgnoredActors.AddUnique(PlayerPawn);
    }

    if (AActor* InstigatorActor = GetInstigator())
    {
        IgnoredActors.AddUnique(InstigatorActor);
    }

    // 2. ���� �� ��� AEnemyCharacterAi ã��
    TArray<AActor*> AllEnemies;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        AEnemyCharacterAi::StaticClass(),
        AllEnemies
    );

    // 3. ���� �� ������ ���� EnemyTakeDamage ȣ��
    for (AActor* Enemy : AllEnemies)
    {
        if (Enemy &&
            FVector::Dist(Enemy->GetActorLocation(), Location) <= ExplosionRadius)
        {
            if (AEnemyCharacterAi* EnemyChar = Cast<AEnemyCharacterAi>(Enemy))
            {
                // �� Ŭ������ EnemyTakeDamage ���� ȣ��
                EnemyChar->EnemyTakeDamage(BulletDamage);
                UE_LOG(LogTemp, Warning, TEXT("Enemy %s took damage"), *EnemyChar->GetName());
            }
        }
    }
    // 4. ����� �� ����Ʈ ó��
    //DrawDebugSphere(GetWorld(), Location, ExplosionRadius, 12, FColor::Red, false, 3.0f);
    Destroy();
}