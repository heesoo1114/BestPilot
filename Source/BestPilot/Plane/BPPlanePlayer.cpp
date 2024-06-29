// Fill out your copyright notice in the Description page of Project Settings.


#include "Plane/BPPlanePlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Object//BPProjectile.h"

ABPPlanePlayer::ABPPlanePlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SocketOffset = FVector(-2200.0f, 0.0f, 800.0f);
	CameraBoom->bUsePawnControlRotation = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>InputMappingContextRef(TEXT("/Game/BestPiot/Input/IMC_Default.IMC_Default"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputActionRollRef(TEXT("/Game/BestPiot/Input/BP_Roll.BP_Roll"));
	if (nullptr != InputActionRollRef.Object)
	{
		RollAction = InputActionRollRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputActionPitchRef(TEXT("/Game/BestPiot/Input/BP_Pitch.BP_Pitch"));
	if (nullptr != InputActionPitchRef.Object)
	{
		PitchAction = InputActionPitchRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputActionYawhRef(TEXT("/Game/BestPiot/Input/BP_Yaw.BP_Yaw"));
	if (nullptr != InputActionYawhRef.Object)
	{
		YawAction = InputActionYawhRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputActionAccelRef(TEXT("/Game/BestPiot/Input/BP_Accel.BP_Accel"));
	if (nullptr != InputActionAccelRef.Object)
	{
		AccelAction = InputActionAccelRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>InputActionDecelRef(TEXT("/Game/BestPiot/Input/BP_Decel.BP_Decel"));
	if (nullptr != InputActionDecelRef.Object)
	{
		DecelAction = InputActionDecelRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>FireActionDecelRef(TEXT("/Game/BestPiot/Input/BP_Fire.BP_Fire"));
	if (nullptr != FireActionDecelRef.Object)
	{
		FireAction = FireActionDecelRef.Object;
	}
}

void ABPPlanePlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ABPPlanePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	// Rotation Input
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ABPPlanePlayer::Roll);
	EnhancedInputComponent->BindAction(PitchAction, ETriggerEvent::Triggered, this, &ABPPlanePlayer::Pitch);
	EnhancedInputComponent->BindAction(YawAction, ETriggerEvent::Triggered, this, &ABPPlanePlayer::Yaw);

	// Move Input
	EnhancedInputComponent->BindAction(AccelAction, ETriggerEvent::Triggered, this, &ABPPlanePlayer::Accel);
	EnhancedInputComponent->BindAction(AccelAction, ETriggerEvent::Completed, this, &ABPPlanePlayer::EndAccel);
	EnhancedInputComponent->BindAction(DecelAction, ETriggerEvent::Triggered, this, &ABPPlanePlayer::Decel);
	EnhancedInputComponent->BindAction(DecelAction, ETriggerEvent::Completed, this, &ABPPlanePlayer::EndDecel);

	// Combat Input
	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &ABPPlanePlayer::Fire);
}

void ABPPlanePlayer::Roll(const FInputActionValue& Value)
{
	float value = Value.Get<float>();
	// UE_LOG(LogTemp, Log, TEXT("Roll Value: %f"), value);
	ProcessRoll(value);
}

void ABPPlanePlayer::Pitch(const FInputActionValue& Value)
{
	float value = Value.Get<float>();
	// UE_LOG(LogTemp, Log, TEXT("Pitch Value: %f"), value);
	ProcessPitch(value);
}

void ABPPlanePlayer::Yaw(const FInputActionValue& Value)
{
	float value = Value.Get<float>();
	ProcessYaw(value);
}

void ABPPlanePlayer::Accel()
{
	if (isDecel) return;
	
	ProcessAccel();
	isAccel = true;
}

void ABPPlanePlayer::Decel()
{
	if (isAccel) return;
	
	ProcessDecel();
	isDecel = true;
}

void ABPPlanePlayer::EndAccel()
{
	isAccel = false;
}

void ABPPlanePlayer::EndDecel()
{
	isDecel = false;
}

void ABPPlanePlayer::Fire()
{
	// Fire Logic
	FVector Location = SpawnPosition->GetComponentLocation();
	FRotator Rotation = SpawnPosition->GetComponentRotation();

	GetWorld()->SpawnActor<ABPProjectile>(Projectile, Location, Rotation);

	UE_LOG(LogTemp, Log, TEXT("Spawn"));
}
