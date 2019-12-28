// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADroneCharacter::ADroneCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DroneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	DroneMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ADroneCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADroneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADroneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("RotatePlayer", this, &ADroneCharacter::RotateDroneYaw);
	PlayerInputComponent->BindAxis("RotateCamera", this, &ADroneCharacter::RotateDronePitch);
	PlayerInputComponent->BindAxis("DroneAscend", this, &ADroneCharacter::Ascend);
	PlayerInputComponent->BindAxis("DroneDescend", this, &ADroneCharacter::Descend);
	PlayerInputComponent->BindAxis("Forward_Backward", this, &ADroneCharacter::MoveForwardBackward);
	PlayerInputComponent->BindAxis("LeftRight", this, &ADroneCharacter::MoveSideways);

}

void ADroneCharacter::Ascend(float AxisValue)
{
	AddMovementInput(GetActorUpVector(), AxisValue);
}

void ADroneCharacter::Descend(float AxisValue)
{
	AddMovementInput(GetActorUpVector(), AxisValue);
}
void ADroneCharacter::MoveForwardBackward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
	UE_LOG(LogTemp, Warning, TEXT("MoveForwardBackward called, AxisValue %f"), AxisValue)
}
void ADroneCharacter::MoveSideways(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void ADroneCharacter::RotateDroneYaw(float AxisValue)
{
	AddControllerYawInput(AxisValue * 0.2f);

}

void ADroneCharacter::RotateDronePitch(float AxisValue)
{
	AddControllerPitchInput(-AxisValue * 0.2f);
}

void ADroneCharacter::CallRotateToDeployed()
{
	RotateToDeployed();
}

void ADroneCharacter::CallRotateToBackPack()
{
	RotateToBackPack();
}

void ADroneCharacter::CallLiftFromBackPack()
{
	LiftFromBackPack();
}