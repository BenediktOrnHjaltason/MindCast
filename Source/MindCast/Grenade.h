// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/DamageType.h"
#include "Grenade.generated.h"

UCLASS()
class MINDCAST_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

	UPROPERTY(VisibleAnywhere)
		USphereComponent* Collider;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* Explosion;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> Damage;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<AActor*> DummyArray;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
