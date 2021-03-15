// Fill out your copyright notice in the Description page of Project Settings.


#include "Temp_Door.h"
#include "Components/SceneComponent.h"

// Sets default values
ATemp_Door::ATemp_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = Root;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	DoorMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATemp_Door::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATemp_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	OpenDoor(DoorCounter);
	UE_LOG(LogTemp, Warning, TEXT("%i"), DoorCounter);
}

void ATemp_Door::PressurePlateEffectStart() 
{
	DoorCounter++;
}

void ATemp_Door::PressurePlateEffectEnd() 
{
	DoorCounter--;
}


void ATemp_Door::OpenDoor(int32 Counter) 
{
	if(Counter >= 2)
	{
		DoorMesh->SetWorldRotation(FRotator(0.0f, 90.0f, 0.0f));
	}
	else
	{
		DoorMesh->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	}
}
