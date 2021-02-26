// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePawn.h"

// Sets default values
ACubePawn::ACubePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CubeCollision"));
	GridCollision->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = GridCollision;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	CubeMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACubePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACubePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACubePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Super::SetupPlayerInputComponent(PlayerInputComponent);

}

