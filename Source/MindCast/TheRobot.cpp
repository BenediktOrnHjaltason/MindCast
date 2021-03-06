// Fill out your copyright notice in the Description page of Project Settings.


#include "TheRobot.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine.h"
#include "DrawDebugHelpers.h"
#include "Engine/DecalActor.h"
#include "PhysicsProp.h"
#include "Grenade.h"
#include "Math/Vector.h"
//#include "Math/Vector.h"


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
	
	CurrentWorld = GetWorld();
	CurrentPlayerController = UGameplayStatics::GetPlayerController(CurrentWorld, 0);

	MuzzleSocketRef = GunRef->GetStaticMesh()->FindSocket(TEXT("Muzzle"));

	CharMoveCompRef = GetCharacterMovement();

	SpawnedDrone = CurrentWorld->SpawnActor<ADroneCharacter>(
		DroneToSpawn, GetMesh()->GetSocketByName("BackpackSocket")->GetSocketLocation(GetMesh()), GetActorForwardVector().Rotation());

	SpawnedDrone->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true), "BackpackSocket");
	SpawnedDroneActor = SpawnedDrone;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATheRobot::Overlaps);
}

// Called every frame
void ATheRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CameraWobble
	MovementVelocity = GetMovementComponent()->Velocity.Size();

	if (MovementVelocity == 0) {
		DeltaTimeMultiplier = 0;
		SineInput = 0;}

	else if (MovementVelocity < 601) {
		DeltaTimeMultiplier = 8;
		SineInput += DeltaTime * DeltaTimeMultiplier;
	}
	else if (MovementVelocity < 1501) {
		DeltaTimeMultiplier = 10;
		SineInput += DeltaTime * DeltaTimeMultiplier;
	}

	UE_LOG(LogTemp,Warning,TEXT("CameraY: %f"), RobotCamera->GetRelativeTransform().GetLocation().Y)

	RobotCamera->SetRelativeLocation(FVector(
		CameraWobbleCenter.X, UKismetMathLibrary::Lerp(CameraWobbleCenter.Y, CameraWobbleEndValue.Y, UKismetMathLibrary::Sin(SineInput)),
			UKismetMathLibrary::Lerp(CameraWobbleCenter.Z, CameraWobbleEndValue.Z, UKismetMathLibrary::Abs(UKismetMathLibrary::Sin(SineInput)))));

	// /CameraWobble
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
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ATheRobot::Shoot);
	PlayerInputComponent->BindAction("DeployDrone", IE_Pressed, this, &ATheRobot::DeployDrone);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &ATheRobot::BuildUpGrenadeVelocity);
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Released, this, &ATheRobot::CallReleaseGrenade);

}

void ATheRobot::MoveForwardAxis(float AxisValue)
{
	AxisInput_ForwardBackward = AxisValue * CharMoveCompRef->MaxWalkSpeed;

	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void ATheRobot::MoveSidewaysAxis(float AxisValue)
{
	AxisInput_Strafe = AxisValue * CharMoveCompRef->MaxWalkSpeed;

	AddMovementInput(GetActorRightVector(), AxisValue);

	//Tilting based on movement
	RobotCamera->SetRelativeRotation(
		UKismetMathLibrary::RLerp(
			FRotator(RobotCamera->GetRelativeTransform().Rotator().Pitch,
				0.f, 0.f),
			FRotator(RobotCamera->GetRelativeTransform().Rotator().Pitch,
				0.f, 3.f), AxisValue,true));
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
}

void ATheRobot::Jump()
{
	ACharacter::Jump();
}

void ATheRobot::StartRunning()
{
	IncreaseFOV();
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;
	
}

void ATheRobot::StopRunning()
{
	DecreaseFOV();
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ATheRobot::Shoot()
{
	MuzzleLoc = GunRef->GetSocketLocation("Muzzle");

	PlayMuzzleEffect();
	JoltArms();
	UGameplayStatics::PlaySound2D(CurrentWorld, RifleSound);

	//CurrentWorld->SpawnActor<AProjectileBase>(ProjectileToSpawn, MuzzleLoc, FVector(TraceEndPoint - MuzzleLoc).ToOrientationRotator()
		//);

	ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	//Fills ViewportCenterWorldLoc and Direction as OutParameter
	CurrentPlayerController->DeprojectScreenPositionToWorld(ViewportSize.X / 2, ViewportSize.Y / 2, ViewportCenterWorldLoc, ViewportCenterDirection);

	TraceEndPoint = (RobotCamera->GetForwardVector() * 5000) + ViewportCenterWorldLoc;

	//DrawDebugLine(CurrentWorld, ViewportCenterWorldLoc + (RobotCamera->GetForwardVector() * 100), TraceEndPoint, FColor::Red, false, 0.1, 0, 2.f);

	AimOffsett = (c_bIsAiming == false) ?  FVector(UKismetMathLibrary::RandomFloatInRange(-160.f, 160.f),
		UKismetMathLibrary::RandomFloatInRange(-160.f, 160.5), UKismetMathLibrary::RandomFloatInRange(-160.f, 160.5)) : 
		FVector(UKismetMathLibrary::RandomFloatInRange(-60.f, 60.f),
			UKismetMathLibrary::RandomFloatInRange(-60.f, 60.f), UKismetMathLibrary::RandomFloatInRange(-60.f, 60.f));

	UE_LOG(LogTemp, Warning, TEXT("AimOffsett is %s"), *AimOffsett.ToString())

	CurrentWorld->LineTraceSingleByChannel(
		LineTraceHit,
		ViewportCenterWorldLoc + (RobotCamera->GetForwardVector() * 100),
		TraceEndPoint + AimOffsett,
		ECollisionChannel::ECC_GameTraceChannel7
	);

	//Outparam vars are in .h
	UGameplayStatics::BreakHitResult(LineTraceHit, bBlockingHit, bInitOverlap, time, distance, TraceHitLocation, ImpactPoint,
		normal, impactNormal, physMat, hitActor, hitComp, hitBoneName, hitItem, faceIndex, traceStart, traceEnd);

	if (hitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *hitActor->GetActorLabel())

			UGameplayStatics::SpawnEmitterAtLocation(
				CurrentWorld, BulletImpact, ImpactPoint, impactNormal.Rotation(), FVector(0.2, 0.2, 0.2), true);

		ADecalActor* NewDecal = CurrentWorld->SpawnActor<ADecalActor>(
			RifleDecalActor, (ImpactPoint + normal * 10), normal.Rotation() + FRotator(-90, 0, 0));

			BulletDecalsQueue.Enqueue(NewDecal);

			//AttachToTarget
			
			NewDecal->AttachToActor(
				hitActor, FAttachmentTransformRules(
					EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true));
					

		BulletQueueCounter++;

		if (BulletQueueCounter > 19) {

			ADecalActor* temp;
			BulletDecalsQueue.Dequeue(temp);
			temp->Destroy();
			BulletQueueCounter--;
		}

		
		if (Cast<APhysicsProp>(hitActor)) {
			HitPhysicsProp = Cast<APhysicsProp>(hitActor);
			UE_LOG(LogTemp, Warning, TEXT("Hit physics prop"))
			ImpulseDirection = ImpactPoint - MuzzleLoc;
			UKismetMathLibrary::Vector_Normalize(ImpulseDirection);

			HitPhysicsProp->Mesh->AddImpulseAtLocation(ImpulseDirection * HitPhysicsProp->Mesh->GetMass() * ImpulseMultiplier, ImpactPoint);
			HitPhysicsProp->Mesh->AddImpulse(ImpulseDirection * HitPhysicsProp->Mesh->GetMass() * 3000);
			HitPhysicsProp = nullptr;
		}
		else if (Cast<AGrenade>(hitActor))
		{
			Cast<AGrenade>(hitActor)->Collider->AddImpulse(FVector(hitActor->GetActorLocation() - MuzzleLoc).GetSafeNormal() * 50000);
		}
			
		hitActor = nullptr;
		
	}
	else	UE_LOG(LogTemp, Warning, TEXT("Not tracing actor"))
	
}

void ATheRobot::SwitchToDroneCamera()
{
	CurrentPlayerController->SetViewTargetWithBlend(SpawnedDroneActor, 1.f, EViewTargetBlendFunction::VTBlend_EaseIn);
}

void ATheRobot::DeployDrone()
{
	DroneLocationBeforeDeploy = SpawnedDrone->GetActorLocation();
	DroneRotationBeforeDeploy = SpawnedDrone->GetActorRotation();

	DisableInput(CurrentPlayerController);
	SwitchToDroneCamera();
	OpenBackpack();
}

void ATheRobot::PossessDrone()
{
	CurrentPlayerController->Possess(SpawnedDrone);
}

void ATheRobot::Overlaps(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(ADroneCharacter::StaticClass())) ReceiveDrone();
}

void ATheRobot::ReceiveDrone()
{
	SpawnedDrone->GetCharacterMovement()->StopMovementImmediately();
	SpawnedDrone->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	SpawnedDrone->DisableInput(CurrentPlayerController);

	CurrentPlayerController->bAutoManageActiveCameraTarget = false;

	CurrentPlayerController->UnPossess();

	CurrentPlayerController->SetViewTarget(SpawnedDrone);
	
	
	

	DroneLocationAtReentry = SpawnedDrone->GetActorLocation();
	DroneRotationAtReentry = SpawnedDrone->GetActorRotation();

	BackPackLoc = GetMesh()->GetSocketLocation("BackpackSocket");
	BackPackAboveLoc = BackPackAboveLoc + FVector(0, 0, 20);

	PlaceDroneInBag();

	//SpawnedDrone->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true), "BackpackSocket");
}

void ATheRobot::LerpDroneAboveBackPack(float Timeline)
{
	SpawnedDrone->SetActorLocation(UKismetMathLibrary::VLerp(DroneLocationAtReentry, DroneLocationBeforeDeploy, Timeline));
	SpawnedDrone->SetActorRotation(
		UKismetMathLibrary::RLerp(DroneRotationAtReentry, DroneRotationBeforeDeploy, Timeline, false));
}

void ATheRobot::LerpDroneIntoBackPack(float Timeline)
{
	SpawnedDrone->SetActorLocation(UKismetMathLibrary::VLerp(BackPackAboveLoc, BackPackLoc, Timeline));
}

void ATheRobot::RepossessRobot()
{
	CurrentPlayerController->Possess(this);
	CurrentPlayerController->SetViewTargetWithBlend(this, 0.3);
	EnableInput(CurrentPlayerController);
}

void ATheRobot::ReleaseGrenade()
{
	FVector CameraLocation = RobotCamera->GetComponentLocation();
	FVector CameraForwardVector = RobotCamera->GetForwardVector();
	AGrenade* SpawnedGrenade = CurrentWorld->SpawnActor<AGrenade>(GrenadeToSpawn, CameraLocation + CameraForwardVector * 150, FRotator(0, 0, 0));

	if (SpawnedGrenade) {
		SpawnedGrenade->Collider->AddImpulse(
			(CameraForwardVector * GrenadeMultiplier * GrenadeMaxVelocity) +
			(CameraForwardVector * GrenadeMinVelocity)
		);
	}
	

}