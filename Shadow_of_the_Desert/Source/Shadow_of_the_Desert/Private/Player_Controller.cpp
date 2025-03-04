// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Controller.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

APlayer_Controller::APlayer_Controller()
	: InPutMappingContext(nullptr),
	MoveAction(nullptr),
	LookAciton(nullptr),
	JumpAciton(nullptr),
	SprintAction(nullptr),
	ReLoadAction(nullptr),
	Swap_Rifle_Hand(nullptr),
	Swap_Rocket_Hand(nullptr),
	Swap_Sinper_Hand(nullptr),
	ShotAction(nullptr)
{
}