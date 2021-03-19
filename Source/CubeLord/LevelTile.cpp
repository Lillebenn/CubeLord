// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTile.h"

// Sets default values
ALevelTile::ALevelTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	TileMesh->SetupAttachment(RootComponent);

	AboveGroundCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AboveGroundCollision"));
	AboveGroundCollision->InitBoxExtent(FVector(50.f));
	RootComponent = AboveGroundCollision;

}

// Called when the game starts or when spawned
void ALevelTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

