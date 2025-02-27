// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy/EnemyCharacterAi.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);
}

FVector AEnemySpawner::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

void AEnemySpawner::EnemySpawn()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundActors);

	TArray<AActor*> ObstacleActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Obstacle"), ObstacleActors);
	
	bool bValidLocation = false;
	FVector SpawnLocation;

	while (!bValidLocation)
	{
		SpawnLocation = GetRandomPointInVolume();

		bValidLocation = true; // ��ȿ�� ��ġ��� ����

		for (AActor* Actor : FoundActors)
		{
			if (Actor)
			{
				float Distance = FVector::Dist(SpawnLocation, Actor->GetActorLocation());
				if (Distance < AvoidanceRadius)
				{
					bValidLocation = false;
					break; // �����̿� �÷��̾ ������ �ٽ� ��ǥ ����
				}
			}
		}

		// Obstacle ���ο� �������� �ʵ��� Ȯ��
		if (bValidLocation) // �÷��̾� ��ó�� �ƴ� ��쿡�� �߰� �˻�
		{
			for (AActor* Obstacle : ObstacleActors)
			{
				if (Obstacle)
				{
					// Obstacle�� ��� �ڽ�(Bounding Box) ��������
					FBox ObstacleBounds = Obstacle->GetComponentsBoundingBox();

					// ���� ��ġ�� Obstacle�� ���ο� �ִ��� Ȯ��
					if (ObstacleBounds.IsInside(SpawnLocation))
					{
						bValidLocation = false; // Obstacle ���ζ�� �ٽ� ã��
						break;
					}
				}
			}
		}
	}
	//���� ���� �ڵ� �߰� or ���� ���� �Լ�
	SpawnRandomEnemy(SpawnLocation);

	// ����׿����� ���� ��ġ�� ȭ�鿡 ǥ�� (5�ʰ� ����, ���)
	DrawDebugSphere(GetWorld(), SpawnLocation, 20.0f, 12, FColor::Green, false, 60.0f);
}

void AEnemySpawner::SpawnRandomEnemy(FVector SpawnLocation)
{
	if (EnemyClasses.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, EnemyClasses.Num() - 1);
		TSubclassOf<AEnemyCharacterAi> SelectedEnemyClass = EnemyClasses[RandomIndex];

		AEnemyCharacterAi* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacterAi>(SelectedEnemyClass, SpawnLocation, FRotator::ZeroRotator);
	}
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

