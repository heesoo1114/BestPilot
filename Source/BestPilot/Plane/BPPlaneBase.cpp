// Fill out your copyright notice in the Description page of Project Settings.


#include "Plane/BPPlaneBase.h"
#include "BPPlaneStatComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Object/BPProjectile.h"

// Sets default values
ABPPlaneBase::ABPPlaneBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collider
	UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->InitBoxExtent(FVector(2000.0f, 500.0f, 500.0f));
	BoxComponent->SetupAttachment(RootComponent);

	// Movement
	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;

	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlaneMeshRef(TEXT(
		"/Game/VigilanteContent/Vehicles/East_Bomber_SU24/SK_East_Bomber_SU24.SK_East_Bomber_SU24"));
	if (PlaneMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(PlaneMeshRef.Object);
	}

	// Stat
	Stat = CreateDefaultSubobject<UBPPlaneStatComponent>(TEXT("Stat"));

	// Effect
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EngineEffect(TEXT(
		"/Game/InfinityBladeEffects/Effects/FX_Mobile/combat/P_MagicSpray_Untyped.P_MagicSpray_Untyped"));
	if (EngineEffect.Succeeded())
	{
		FireEffectComponent_L = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EngineEffectComponent_L"));
		FireEffectComponent_R = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EngineEffectComponent_R"));

		FireEffectComponent_L->SetTemplate(EngineEffect.Object);
		FireEffectComponent_R->SetTemplate(EngineEffect.Object);

		FireEffectComponent_L->SetupAttachment(GetMesh());
		FireEffectComponent_R->SetupAttachment(GetMesh());

		FireEffectComponent_L->SetRelativeLocation(FVector(-960.0f, -70.0f, 200.0f));
		FireEffectComponent_R->SetRelativeLocation(FVector(-960.0f, +70.0f, 200.0f));

		FireEffectComponent_L->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
		FireEffectComponent_R->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
	}

	// Combat 
	SpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPosition"));
	SpawnPosition->SetupAttachment(GetMesh());
	SpawnPosition->SetRelativeLocation(FVector(1300.0f, -20.0f, 210.0f));

	static ConstructorHelpers::FClassFinder<ABPProjectile> ProjectileClassRef(TEXT(
		"/Game/BestPiot/BP_Object/BP_Projectile.BP_Projectile_C"));
	if (ProjectileClassRef.Class)
	{
		Projectile = ProjectileClassRef.Class;
	}
}

// Called when the game starts or when spawned
void ABPPlaneBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABPPlaneBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Calculate Rotation
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Roll = CurrentRollSpeed * DeltaTime;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaTime;
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaTime;

	FRotator NewRotation = GetActorRotation() + DeltaRotation;
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -65.0f, 65.0f);
	// NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw, -180.0f, 180.0f);
	// NewRotation.Roll = FMath::Fmod(NewRotation.Roll, 360.0f);
	NewRotation.Roll = FMath::Clamp(NewRotation.Roll, -60.0f, 60.0f);

	SetActorRotation(NewRotation);

	if (false == bIntentionalPitch)
	{
		CurrentPitchSpeed = 0.0f;
	}

	if (false == bIntentionalRoll)
	{
		CurrentRollSpeed = 0.0f;
	}

	// Calculate Thrust
	const float CurrentArc = -GetActorRotation().Pitch * DeltaTime * Stat->Acceleration;
	const float NewForwardSpeed = Stat->CurrentForwardSpeed + CurrentArc;
	Stat->CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, Stat->MinSpeed, Stat->MaxSpeed);

	const FVector LocalMove = FVector(Stat->CurrentForwardSpeed * DeltaTime, 0.0f, 0.0f);
	AddActorLocalOffset(LocalMove, true);
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
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 0.75f);
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
		RollSpeed = GetActorRotation().Roll * (-1.5f);
	}

	const float TargetRollSpeed = RollSpeed;
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 0.75f);
}

void ABPPlaneBase::ProcessYaw(float value)
{
	const float TargetYawSpeed = value * Stat->YawRateMultiplier;
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 0.75f);

	ProcessRoll(value);
}

void ABPPlaneBase::ProcessAccel()
{
	float addAmount = Stat->CurrentForwardSpeed * 0.01f;
	Stat->CurrentForwardSpeed = FMath::Clamp(Stat->CurrentForwardSpeed + addAmount, Stat->MinSpeed, Stat->MaxSpeed);
	// UE_LOG(LogTemp, Log, TEXT("Accel : %f"), addAmount);
}

void ABPPlaneBase::ProcessDecel()
{
	float addAmount = Stat->CurrentForwardSpeed * 0.01f;
	Stat->CurrentForwardSpeed = FMath::Clamp(Stat->CurrentForwardSpeed - addAmount, Stat->MinSpeed, Stat->MaxSpeed);
	// UE_LOG(LogTemp, Log, TEXT("Decel : %f"), addAmount);
}
