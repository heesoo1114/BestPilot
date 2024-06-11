// Fill out your copyright notice in the Description page of Project Settings.


#include "Plane/BPPlanePlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ABPPlanePlayer::ABPPlanePlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SocketOffset = FVector(-2200.0f, 0.0f, 800.0f);
	CameraBoom->bUsePawnControlRotation = true;

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
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ABPPlanePlayer::Roll);
	EnhancedInputComponent->BindAction(PitchAction, ETriggerEvent::Triggered, this, &ABPPlanePlayer::Pitch);
	EnhancedInputComponent->BindAction(YawAction, ETriggerEvent::Triggered, this, &ABPPlanePlayer::Yaw);
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
