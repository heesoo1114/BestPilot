// Fill out your copyright notice in the Description page of Project Settings.


#include "Plane/BPPlayerController.h"

ABPPlayerController::ABPPlayerController()
{

}

void ABPPlayerController::BeginPlay()
{
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
