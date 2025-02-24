// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
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

	const float AvoidanceRadius = 300.0f;
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
					// ���� �Ʒ��� ���� ���� �ڵ� �߰� or ���� ���� �Լ�

					break; // ����� �÷��̾ ������ �ٽ� ��ǥ ����
				}
			}
		}
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

