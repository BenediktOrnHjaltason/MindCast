// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("USphereComponent"));
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	DummyArray.Init(this, 1);

	GetWorldTimerManager().SetTimer(ExplosionHandle, this, &AGrenade::Explode, 4, false);
	CurrentWorld = GetWorld();
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AGrenade::Explode()
{
	FVector Location = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(CurrentWorld, Explosion, Location, FRotator(0, 0, 0), FVector(4,4,4),true);
	UGameplayStatics::SpawnEmitterAtLocation(CurrentWorld, ShockWave, Location, FRotator(0, 0, 0), FVector(4,4,4), true);
	UGameplayStatics::PlaySoundAtLocation(CurrentWorld, ExplosionSound, Location);
	UGameplayStatics::ApplyRadialDamage(CurrentWorld, 25000.f, GetActorLocation(), 5000, Damage, DummyArray,this);
	Destroy();
}
