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

		bValidLocation = true; // 유효한 위치라고 가정

		for (AActor* Actor : FoundActors)
		{
			if (Actor)
			{
				float Distance = FVector::Dist(SpawnLocation, Actor->GetActorLocation());
				if (Distance < AvoidanceRadius)
				{
					bValidLocation = false;
					break; // 가까이에 플레이어가 있으면 다시 좌표 생성
				}
			}
		}

		// Obstacle 내부에 스폰되지 않도록 확인
		if (bValidLocation) // 플레이어 근처가 아닌 경우에만 추가 검사
		{
			for (AActor* Obstacle : ObstacleActors)
			{
				if (Obstacle)
				{
					// Obstacle의 경계 박스(Bounding Box) 가져오기
					FBox ObstacleBounds = Obstacle->GetComponentsBoundingBox();

					// 스폰 위치가 Obstacle의 내부에 있는지 확인
					if (ObstacleBounds.IsInside(SpawnLocation))
					{
						bValidLocation = false; // Obstacle 내부라면 다시 찾기
						break;
					}
				}
			}
		}
	}
	//몬스터 스폰 코드 추가 or 몬스터 생성 함수
	SpawnRandomEnemy(SpawnLocation);

	// 디버그용으로 스폰 위치를 화면에 표시 (5초간 유지, 녹색)
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

