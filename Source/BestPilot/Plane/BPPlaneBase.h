// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BPPlaneBase.generated.h"

UCLASS()
class BESTPILOT_API ABPPlaneBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABPPlaneBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Rotation Section
protected:

	float CurrentYawSpeed;
	float CurrentPitchSpeed;
	float CurrentRollSpeed;

	bool bIntentionalRoll = false;
	bool bIntentionalPitch = false;

	void ProcessKeyPitch(float rate);
	void ProcessKeyRoll(float rate);

	// 회전값을 계산하는 함수
	void ProcessPitch(float value); // 좌우 축(Y축)을 기준으로 회전값 계산
	void ProcessRoll(float value); // 상하 축(X축)을 기준으로 회전값 계산
	void ProcessYaw(float value); 

	// 가속
	void ProcessAccel();
	void ProcessDecel();

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<class UBPPlaneStatComponent> Stat;

// Effect Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> FireEffectComponent_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> FireEffectComponent_R;

// Combat Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	TObjectPtr<class USceneComponent> SpawnPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	TSubclassOf<class ABPProjectile> Projectile;

// Audio Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<class UAudioComponent> LoopingAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	TObjectPtr<class USoundCue> EngineSound;
};
