// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../Public/Weapon/WeaponBase.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Weapon/Rifle.h"
#include "Weapon/Sniper.h"
#include "Weapon/RocketLauncher.h"
#include "Shadow_of_the_DesertCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AWeaponBase;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AShadow_of_the_DesertCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	AShadow_of_the_DesertCharacter();
	float GetHelth();
	float GetMaxHelth();
	AWeaponBase* Weapon;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	bool Ues_Rifle_now;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	bool Ues_Sniper_now;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	bool Ues_Rocket_now;

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;
	void EquipWeapon(TSubclassOf<AWeaponBase> WeaponClass);
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	bool Ues_Rifle_now;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	bool Ues_Sniper_now;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	bool Ues_Rocket_now;

	AWeaponBase* GetEquippedWeapon();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeaponBase* EquippedWeapon;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ARifle> RifleClass;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ASniper> SniperClass;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ARocketLauncher> RocketLauncherClass;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Health")
	float Health;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Defensive")
	float MaxDefensive;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Defensive")
	float Defensive;

	
	void BeginPlay() override;

	/** Called for movement input */
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void Shot(const FInputActionValue& value);
	UFUNCTION()
	void Reload(const FInputActionValue& value);
	UFUNCTION()
	void Swap_Rifle(const FInputActionValue& value);
	UFUNCTION()
	void Swap_Sinper(const FInputActionValue& value);
	UFUNCTION()
	void Swap_Rocket(const FInputActionValue& value);




	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float Speed;
	float SprintSpeedMultiplier;
	float SprintSpeed;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

