// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Collider);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("RotateCamera", this, &ADrone::RotateDrone);
	PlayerInputComponent->BindAxis("DroneAscend",  this, &ADrone::Ascend);
	PlayerInputComponent->BindAxis("DroneDescend", this, &ADrone::Descend);
	PlayerInputComponent->BindAxis("Forward_Backward", this, &ADrone::MoveForwardBackward);
	PlayerInputComponent->BindAxis("LeftRight", this, &ADrone::MoveForwardBackward);

}

void ADrone::Ascend(float AxisValue)
{
	SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, AxisValue) * MovementMultiplier);
}

void ADrone::Descend(float AxisValue)
{
	SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, AxisValue) * MovementMultiplier);
}
void ADrone::MoveForwardBackward(float AxisValue)
{
	SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, AxisValue) * MovementMultiplier);
}
void ADrone::MoveSideways(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void ADrone::RotateDrone(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

