// Fill out your copyright notice in the Description page of Project Settings.


#include "SmallGate.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASmallGate::ASmallGate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ASmallGate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmallGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmallGate::HandleDestruction() 
{
	if (DeathParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
	}
	Mesh->DestroyComponent();
	DestroyGate();
}

