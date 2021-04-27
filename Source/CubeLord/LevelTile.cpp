// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTile.h"
#include "Albert_Character.h"
#include "Components/PrimitiveComponent.h"

#define COLLISION_MAGNETICCUBE ECC_GameTraceChannel2

// Sets default values
ALevelTile::ALevelTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AboveGroundCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AboveGroundCollision"));
	AboveGroundCollision->InitBoxExtent(FVector(50.f));
	RootComponent = AboveGroundCollision;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile"));
	TileMesh->SetupAttachment(RootComponent);
}
/*
// If the player is standing on a tile, a magnetic cube cannot enter that tile.
void ALevelTile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AAlbert_Character::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Block Cube!"));
		AboveGroundCollision->UPrimitiveComponent::SetCollisionResponseToChannel(COLLISION_MAGNETICCUBE, ECR_Block);
		AboveGroundCollision->SetVisibility(true);
	}
}

// Resets the collision response when the player leaves the tile.
void ALevelTile::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AAlbert_Character::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("No Block Cube!"));
		
		AboveGroundCollision->UPrimitiveComponent::SetCollisionResponseToChannel(COLLISION_MAGNETICCUBE, ECR_Overlap);
		AboveGroundCollision->SetVisibility(false);
	}
}*/

void ALevelTile::SetBlockResponse()
{
	AboveGroundCollision->UPrimitiveComponent::SetCollisionResponseToChannel(COLLISION_MAGNETICCUBE, ECR_Block);
	AboveGroundCollision->SetVisibility(true);
}

void ALevelTile::ResetCollisionResponse()
{
	AboveGroundCollision->UPrimitiveComponent::SetCollisionResponseToChannel(COLLISION_MAGNETICCUBE, ECR_Overlap);
	AboveGroundCollision->SetVisibility(false);
}

// Called when the game starts or when spawned
void ALevelTile::BeginPlay()
{
	Super::BeginPlay();
	// AboveGroundCollision->OnComponentBeginOverlap.AddDynamic(this, &ALevelTile::OnBeginOverlap);
	// AboveGroundCollision->OnComponentEndOverlap.AddDynamic(this, &ALevelTile::OnEndOverlap);
}

// Called every frame
void ALevelTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

