// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/BPProjectile.h"

// Sets default values
ABPProjectile::ABPProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;
}

// Called when the game starts or when spawned
void ABPProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABPProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

