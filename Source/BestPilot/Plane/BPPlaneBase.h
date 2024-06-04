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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat)
	TObjectPtr<class UBPPlaneStatComponent> Stat;

	void ProcessKeyPitch(float value);
	void ProcessKeyRoll(float value);

	// 회전값을 계산하는 함수
	void ProcessPitch(float value); // 좌우 축(Y축)을 기준으로 회전값 계산
	void ProecessRoll(float value); // 상하 축(X축)을 기준으로 회전값 계산

};
