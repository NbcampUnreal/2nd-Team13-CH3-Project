// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player_Controller.generated.h"


class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SHADOW_OF_THE_DESERT_API APlayer_Controller : public APlayerController
{
	GENERATED_BODY()

public:
	APlayer_Controller();
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InPutMappingContext;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAciton;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAciton;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ReLoadAction;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ShotAction;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Swap_Rifle_Hand;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Swap_Sinper_Hand;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	UInputAction* Swap_Rocket_Hand;
};
