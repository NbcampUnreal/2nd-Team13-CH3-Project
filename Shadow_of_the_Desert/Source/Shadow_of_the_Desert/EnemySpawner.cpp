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

		bValidLocation = true; // 유효한 위치라고 가정

		for (AActor* Actor : FoundActors)
		{
			if (Actor)
			{
				float Distance = FVector::Dist(SpawnLocation, Actor->GetActorLocation());
				if (Distance < AvoidanceRadius)
				{
					bValidLocation = false;
					// 여기 아래에 몬스터 스폰 코드 추가 or 몬스터 생성 함수

					break; // 가까운 플레이어가 있으면 다시 좌표 생성
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

