// Fill out your copyright notice in the Description page of Project Settings.


#include "Plane/BPPlaneBase.h"
#include "BPPlaneStatComponent.h"

// Sets default values
ABPPlaneBase::ABPPlaneBase()
{
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
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABPPlaneBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABPPlaneBase::ProcessKeyPitch(float value)
{
}

void ABPPlaneBase::ProcessKeyRoll(float value)
{
}

void ABPPlaneBase::ProcessPitch(float value)
{
}

void ABPPlaneBase::ProecessRoll(float value)
{
}

