// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePawn.h"
#include "Math/UnrealMathUtility.h"

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

FVector ACubePawn::FindNearestDirection(float Xin, float Yin)
{
	float tempX = Xin;
	float tempY = Yin;
	float tempXlength;
	float tempYlength;
	bool Xnegative{ false };
	bool Ynegative{ false };
	FVector tempVec;
	
	if (FMath::FloorToInt(tempX) == 0 ||  FMath::FloorToInt(tempY) == 0)
	{
		return tempVec;
	}
	
	if (tempX < 0)
	{
		tempXlength = tempX * -1;
		Xnegative = true;
	}
	else 
	{
		tempXlength = tempX;
	}
	
	if (tempY < 0)
	{
		tempYlength = tempY * -1;
		Ynegative = true;
	}
	else
	{
		tempYlength = tempY;
	}

	if (tempXlength > tempYlength)
	{
		if (Xnegative == true)
		{
			tempVec.X = -1;
			tempVec.Y = 0;

			return tempVec;
		}
		else
		{
			tempVec.X = 1;
			tempVec.Y = 0;

			return tempVec;
		}
	}
	else
	{
		if (Ynegative == true)
		{
			tempVec.X = 0;
			tempVec.Y = -1;

			return tempVec;
		}
		else
		{
			tempVec.X = 0;
			tempVec.Y = 1;

			return tempVec;
		}
	}
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

