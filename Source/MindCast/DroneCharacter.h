// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DroneCharacter.generated.h"

UCLASS()
class MINDCAST_API ADroneCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADroneCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* DroneMesh;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* Camera;

	void Ascend(float AxisValue);
	void Descend(float AxisValue);
	void MoveForwardBackward(float AxisValue);
	void MoveSideways(float AxisValue);
	void RotateDroneYaw(float AxisValue);
	void RotateDronePitch(float AxisValue);

	UFUNCTION(BlueprintImplementableEvent)
		void RotateToDeployed();
	UFUNCTION(BlueprintCallable)
		void CallRotateToDeployed();

	UFUNCTION(BlueprintImplementableEvent)
		void RotateToBackPack();
	UFUNCTION(BlueprintCallable)
		void CallRotateToBackPack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
