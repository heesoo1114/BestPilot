// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BESTPILOT_API ABPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABPPlayerController();

protected:
	virtual void BeginPlay() override;
	
};
