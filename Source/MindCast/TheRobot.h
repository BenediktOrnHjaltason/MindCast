// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TheRobot.generated.h"

UCLASS()
class MINDCAST_API ATheRobot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATheRobot();



	UPROPERTY(EditAnywhere)
	UCameraComponent* RobotCamera;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Arms;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForwardAxis(float AxisValue);
	void MoveSidewaysAxis(float AxisValue);
	void RotatePlayer(float AxisValue);
	void RotateCamera(float AxisValue);
	void Jump();
	void StartRunning();
	void StopRunning();

	UFUNCTION(BlueprintImplementableEvent)
	void StartAiming();

	UFUNCTION(BlueprintImplementableEvent)
	void StopAiming();

	UCharacterMovementComponent* OurMovementComponent;

	UPROPERTY(EditAnywhere)
	float CameraPitch{ 0.f };
	

private:

	FVector V{ 0.f, 0.f, 0.f };
	float m_speed{ 0.f };
	
};
