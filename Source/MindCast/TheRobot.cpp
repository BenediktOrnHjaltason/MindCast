// Fill out your copyright notice in the Description page of Project Settings.


#include "TheRobot.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATheRobot::ATheRobot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RobotCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RobotCamera->SetupAttachment(RootComponent);

	Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arms"));
	Arms->SetupAttachment(RobotCamera);
}

// Called when the game starts or when spawned
void ATheRobot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATheRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	V = GetCharacterMovement()->Velocity;
	m_speed = UKismetMathLibrary::Sqrt(UKismetMathLibrary::MultiplyMultiply_FloatFloat(V.X, 2.f) +
		UKismetMathLibrary::MultiplyMultiply_FloatFloat(V.Y, 2.f) +
		UKismetMathLibrary::MultiplyMultiply_FloatFloat(V.Z, 2.f));


	UE_LOG(LogTemp, Warning, TEXT("Camera pitch: %f"), RobotCamera->GetRelativeTransform().Rotator().Pitch)
}

// Called to bind functionality to input
void ATheRobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward_Backward", this, &ATheRobot::MoveForwardAxis);
	PlayerInputComponent->BindAxis("LeftRight", this, &ATheRobot::MoveSidewaysAxis);
	PlayerInputComponent->BindAxis("RotatePlayer", this, &ATheRobot::RotatePlayer);
	PlayerInputComponent->BindAxis("RotateCamera", this, &ATheRobot::RotateCamera);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATheRobot::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATheRobot::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ATheRobot::StopRunning);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATheRobot::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATheRobot::StopAiming);
	

}

void ATheRobot::MoveForwardAxis(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void ATheRobot::MoveSidewaysAxis(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void ATheRobot::RotatePlayer(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void ATheRobot::RotateCamera(float AxisValue)
{

	CameraPitch = RobotCamera->GetRelativeTransform().Rotator().Pitch;

	if( CameraPitch >= -73.5f && CameraPitch <= 73.f)
		RobotCamera->SetRelativeRotation(RobotCamera->GetRelativeTransform().Rotator() + FRotator(AxisValue, 0.0, 0.0));

	else if (CameraPitch < -73.5f) RobotCamera->SetRelativeRotation(
		FRotator(-72.f, RobotCamera->GetRelativeTransform().Rotator().Yaw, RobotCamera->GetRelativeTransform().Rotator().Roll));

	else if (CameraPitch > 73.f) RobotCamera->SetRelativeRotation(
		FRotator(72.5f, RobotCamera->GetRelativeTransform().Rotator().Yaw, RobotCamera->GetRelativeTransform().Rotator().Roll));
	
	//-83.5 - 76.0
}

void ATheRobot::Jump()
{
	ACharacter::Jump();
}

void ATheRobot::StartRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;
}

void ATheRobot::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

/*
void ATheRobot::StartAiming()
{
	bIsAiming = true;
}

void ATheRobot::StopAiming()
{
	bIsAiming = false;
}
*/