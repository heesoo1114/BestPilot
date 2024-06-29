// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BPPlaneStatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BESTPILOT_API UBPPlaneStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBPPlaneStatComponent();

protected:
	virtual void InitializeComponent() override;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Plane")
	float Acceleration{ 38.0f }; 
	
	UPROPERTY(EditAnywhere, Category = "Plane")
	float MaxSpeed{ 6000.f };
	UPROPERTY(EditAnywhere, Category = "Plane")
	float MinSpeed{ 1000.f };
	
	UPROPERTY(EditAnywhere, Category = "Plane")
	float CurrentForwardSpeed{ 2000.f };

	UPROPERTY(EditAnywhere, Category = "Plane")
	float RollRotateMultiplier{ 200.f };
	UPROPERTY(EditAnywhere, Category = "Plane")
	float PitchRateMultiplier{ 200.f };
	UPROPERTY(EditAnywhere, Category = "Plane")
	float YawRateMultiplier{ 2000.f };

};
