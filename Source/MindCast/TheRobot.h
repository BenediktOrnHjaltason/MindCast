// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "ProjectileBase.h"
#include "Materials/MaterialInterface.h"
#include "Sound/SoundBase.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* Arms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GunRef;

	UPROPERTY(EditAnywhere, Category = Sound)
		USoundBase* RifleSound;

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
	void Shoot();

	UFUNCTION(BlueprintImplementableEvent)
		void StartAiming();

	UFUNCTION(BlueprintImplementableEvent)
		void StopAiming();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayMuzzleEffect();

	UFUNCTION(BlueprintImplementableEvent)
		void JoltArms();

	UCharacterMovementComponent* OurMovementComponent;

	UWorld* CurrentWorld{ nullptr };
	FHitResult LineTraceHit;
	APlayerController* CurrentPlayerController;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	FVector2D ViewportSize{ 0,0 };
	FVector ViewportCenterWorldLoc{ 0,0,0 };
	FVector ViewportCenterDirection{ 0,0,0 };
	FVector TraceHitLocation{ 0,0,0 };
	FVector ImpactPoint{ 0,0,0 };
	FVector TraceEndPoint{ 0,0,0 };
	FRotator ProjectileRotation{ 0.f,0.f,0.f };

	//Set from blueprint (Didn't get socket functionality on mesh added from C++)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector MuzzleLoc;

	UStaticMeshSocket* MuzzleSocketRef;

	UPROPERTY(EditAnywhere)
		float CameraPitch{ 0.f };

	//------HitResultVars
	bool bBlockingHit{false};
	bool bInitOverlap{ false };
	float time{ 0.f };
	float distance{ 0.f };
	FVector normal{ 0.f, 0.f, 0.f };
	FVector impactNormal{ 0.f, 0.f, 0.f };
	UPhysicalMaterial* physMat{ nullptr };
	AActor* hitActor{ nullptr };
	UPrimitiveComponent* hitComp{ nullptr };
	FName hitBoneName{ "" };
	int32 hitItem{ 0 };
	int32 faceIndex{ 0 };
	FVector traceStart{ 0 };
	FVector traceEnd{ 0 };
	//---/HitResultVars


	//--Projectile--//
	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<AProjectileBase>ProjectileToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
		UMaterialInterface* RifleBulletHole;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ADecalActor> RifleDecalActor;

	//Only last ten BulletHoles remain
	TQueue<ADecalActor*> BulletDecalsQueue;

	int BulletQueueCounter{ 0 };

	class APhysicsProp* HitPhysicsProp{nullptr};

	float ImpulseLength{ 0 };
	FVector ImpulseDirection{ 0,0,0 };

	//--Movent--//

	UCharacterMovementComponent* CharMoveCompRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AxisInput_ForwardBackward{ 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AxisInput_Strafe{ 0.f };

private:

	
};