// Copyright Epic Games, Inc. All Rights Reserved.

#include "Shadow_of_the_DesertCharacter.h"
#include "Shadow_of_the_DesertGameState.h"
#include "../Public/Weapon/Rifle.h"
#include "Engine/LocalPlayer.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Player_Controller.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AShadow_of_the_DesertCharacter

AShadow_of_the_DesertCharacter::AShadow_of_the_DesertCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	PrimaryActorTick.bCanEverTick = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 550.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Speed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = Speed * SprintSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = Speed;

	MaxHealth = 100.0f;
	Health = MaxHealth;
	bIsfirst = true;

	Ues_Rifle_now = true;
	Ues_Sniper_now = false;
	Ues_Rocket_now = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

}

void AShadow_of_the_DesertCharacter::BeginPlay()
{
	Super::BeginPlay();

	// �⺻ ���� ����
	if (RifleClass) // RifleClass�� �����Ǿ� �ִ� ���
	{
		EquipWeapon(RifleClass);

		// ���Ⱑ ����� �����Ǿ����� Ȯ��
		if (EquippedWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon equipped: %s"), *EquippedWeapon->GetName());
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AShadow_of_the_DesertCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayer_Controller* PlayerController = Cast<APlayer_Controller>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerController->InPutMappingContext, 0);
		}
	}
}

void AShadow_of_the_DesertCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (APlayer_Controller* PlayerController = Cast<APlayer_Controller>(GetController())) {
			if (PlayerController->MoveAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AShadow_of_the_DesertCharacter::Move);
			}
			if (PlayerController->LookAciton)
			{
				EnhancedInputComponent->BindAction(PlayerController->LookAciton, ETriggerEvent::Triggered, this, &AShadow_of_the_DesertCharacter::Look);			}
			if (PlayerController->JumpAciton)
			{
				EnhancedInputComponent->BindAction(PlayerController->JumpAciton, ETriggerEvent::Triggered, this, &AShadow_of_the_DesertCharacter::StartJump);
				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed, this, &AShadow_of_the_DesertCharacter::StopJump);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Triggered, this, &AShadow_of_the_DesertCharacter::StartSprint);
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed, this, &AShadow_of_the_DesertCharacter::StopSprint);
			}
			if (PlayerController->ShotAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->ShotAction, ETriggerEvent::Triggered, this, &AShadow_of_the_DesertCharacter::Shot);
			}
			if (PlayerController->ReLoadAction)
			{
				EnhancedInputComponent->BindAction(PlayerController->ReLoadAction, ETriggerEvent::Triggered, this, &AShadow_of_the_DesertCharacter::Reload);
			}
			if (PlayerController->Swap_Rifle_Hand)
			{
				EnhancedInputComponent->BindAction(PlayerController->Swap_Rifle_Hand, ETriggerEvent::Triggered, this, &AShadow_of_the_DesertCharacter::Swap_Rifle);

			}
			if (PlayerController->Swap_Sinper_Hand)
			{
				EnhancedInputComponent->BindAction(PlayerController->Swap_Sinper_Hand, ETriggerEvent::Triggered, this, &AShadow_of_the_DesertCharacter::Swap_Sinper);

			}
			if (PlayerController->Swap_Rocket_Hand)
			{
				EnhancedInputComponent->BindAction(PlayerController->Swap_Rocket_Hand, ETriggerEvent::Triggered, this, &AShadow_of_the_DesertCharacter::Swap_Rocket);

			}
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AShadow_of_the_DesertCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D

	if (!Controller) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MovementVector.X))
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.X);
	}

	if (!FMath::IsNearlyZero(MovementVector.Y))
	{
		AddMovementInput(GetActorRightVector(), MovementVector.Y);
	}
}

void AShadow_of_the_DesertCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AShadow_of_the_DesertCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}

void AShadow_of_the_DesertCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void AShadow_of_the_DesertCharacter::StartSprint(const FInputActionValue& value) 
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void AShadow_of_the_DesertCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
}
void AShadow_of_the_DesertCharacter::Shot(const FInputActionValue& value)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Attack();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon equipped!"));
	}
}


void AShadow_of_the_DesertCharacter::Reload(const FInputActionValue& value)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Reload();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No weapon equipped!"));
	}
}
void AShadow_of_the_DesertCharacter::Swap_Rifle(const FInputActionValue& value)
{
	EquipWeapon(RifleClass);
	Ues_Rifle_now = true;
	Ues_Sniper_now = false;
	Ues_Rocket_now = false;
}

void AShadow_of_the_DesertCharacter::Swap_Sinper(const FInputActionValue& value)
{
	EquipWeapon(SniperClass);
	Ues_Rifle_now = false;
	Ues_Sniper_now = true;
	Ues_Rocket_now = false;
}

void AShadow_of_the_DesertCharacter::Swap_Rocket(const FInputActionValue& value)
{
	EquipWeapon(RocketLauncherClass);
	Ues_Rifle_now = false;
	Ues_Sniper_now = false;
	Ues_Rocket_now = true;
}

float AShadow_of_the_DesertCharacter::GetHelth() {
	return Health;
}
float AShadow_of_the_DesertCharacter::GetMaxHelth()
{
	return MaxHealth;
}

float AShadow_of_the_DesertCharacter::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	if (AShadow_of_the_DesertGameState* SOTDGameState = Cast<AShadow_of_the_DesertGameState>(GetWorld()->GetGameState()))
	{
		SOTDGameState->SetTakenDamage(DamageAmount);
		if (Health == 0.0)
		{
			SOTDGameState->GameEnd("Defeat");
		}
	}

	return ActualDamage;
}

void AShadow_of_the_DesertCharacter::EquipWeapon(TSubclassOf<AWeaponBase> WeaponClass)
{
	if (WeaponClass)
	{
		if (EquippedWeapon && EquippedWeapon->IsA(WeaponClass))
		{
			return;
		}

		if (EquippedWeapon)
		{
			if (EquippedWeapon->bIsReloading)
			{
				EquippedWeapon->CancelReload();
			}

			if (EquippedWeapon->IsA(ARifle::StaticClass())) {
				RifleAmmo = EquippedWeapon->CurrentAmmo;
			}
			else if (EquippedWeapon->IsA(ASniper::StaticClass())) {
				SniperAmmo = EquippedWeapon->CurrentAmmo;
			}
			else if (EquippedWeapon->IsA(ARocketLauncher::StaticClass())) {
				RocketLauncherAmmo = EquippedWeapon->CurrentAmmo;
			}
			EquippedWeapon->Destroy();
			EquippedWeapon = nullptr;
		}

		EquippedWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass);

		if (WeaponClass->IsChildOf(ARifle::StaticClass())) {
			EquippedWeapon->CurrentAmmo = FMath::Clamp(RifleAmmo, 0, EquippedWeapon->MaxAmmo);
		}
		else if (WeaponClass->IsChildOf(ASniper::StaticClass())) {
			EquippedWeapon->CurrentAmmo = FMath::Clamp(SniperAmmo, 0, EquippedWeapon->MaxAmmo);
		}
		else if (WeaponClass->IsChildOf(ARocketLauncher::StaticClass())) {
			EquippedWeapon->CurrentAmmo = FMath::Clamp(RocketLauncherAmmo, 0, EquippedWeapon->MaxAmmo);
		}

		if (EquippedWeapon)
		{
			EquippedWeapon->bCanAttack = false;

			GetWorld()->GetTimerManager().SetTimer(
				EquippedWeapon->AttackCooldownHandle,
				[this]() {
					if (EquippedWeapon)
						EquippedWeapon->bCanAttack = true;
				},
				1.0f, // Delay
				false
			);



			USkeletalMeshComponent* SkeletalMesh = GetMesh();
			if (SkeletalMesh)
			{
				EquippedWeapon->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_l"));

				FRotator NewRotation = GetActorRotation();
				EquippedWeapon->SetActorRotation(NewRotation + FRotator(0.0f, 0.0f, 0.0f));
			}
		}
	}
}

AWeaponBase* AShadow_of_the_DesertCharacter::GetEquippedWeapon() {
	return EquippedWeapon;
}