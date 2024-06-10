// Fill out your copyright notice in the Description page of Project Settings.


#include "Plane/BPPlaneBase.h"
#include "BPPlaneStatComponent.h"

// Sets default values
ABPPlaneBase::ABPPlaneBase()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlaneMeshRef(TEXT(
		"/Game/VigilanteContent/Vehicles/East_Bomber_SU24/SK_East_Bomber_SU24.SK_East_Bomber_SU24"));
	if (PlaneMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(PlaneMeshRef.Object);
	}

	// Stat
	Stat = CreateDefaultSubobject<UBPPlaneStatComponent>(TEXT("Stat"));
}

// Called when the game starts or when spawned
void ABPPlaneBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABPPlaneBase::Tick(float DeltaTime)
{
	// Calculate Thrust
	const float CurrentArc = -GetActorRotation().Pitch * DeltaTime * Stat->Acceleration;
	const float NewForwardSpeed = Stat->CurrentForwardSpeed * CurrentArc;
	Stat->CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, Stat->MinSpeed, Stat->MaxSpeed);
	
	const FVector LocalMove = FVector(Stat->CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);
	AddActorLocalOffset(LocalMove);

	FRotator DeltaRotation(0, 0, 0); 
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime; 
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;

	AddActorLocalRotation(DeltaRotation);

	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABPPlaneBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABPPlaneBase::ProcessKeyPitch(float rate)
{
	if (FMath::Abs(rate) > 0.2f)
	{
		ProcessPitch(rate * 2.0f);
	}
}

void ABPPlaneBase::ProcessKeyRoll(float rate)
{
	if (FMath::Abs(rate) > 0.2f)
	{
		ProcessRoll(rate * 2.0f);
	}
}

void ABPPlaneBase::ProcessPitch(float value)
{
	bIntentionalPitch = FMath::Abs(value) > 0.0f;

	const float TargetPitchSpeed = value * Stat->PitchRateMultiplier;
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void ABPPlaneBase::ProcessRoll(float value)
{
	bIntentionalRoll = FMath::Abs(value) > 0.0f;

	if (bIntentionalPitch && (false == bIntentionalRoll))
	{
		return;
	}

	float RollSpeed = 0.0f;
	if (bIntentionalRoll)
	{
		RollSpeed = value * Stat->RollRotateMultiplier;
	}
	else
	{
		RollSpeed = GetActorRotation().Roll * (-2.0f);
	}

	const float TargetRollSpeed = RollSpeed;
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.0f);
}

