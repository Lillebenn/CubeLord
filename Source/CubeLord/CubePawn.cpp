// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePawn.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "LevelTile.h"

#define COLLISION_MAGNETICCUBE ECC_GameTraceChannel2

// Sets default values
ACubePawn::ACubePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CubeCollision"));
	GridCollision->InitBoxExtent(FVector(50.f));
	RootComponent = GridCollision;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	CubeMesh->SetupAttachment(RootComponent);

	this->MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));
	this->MovementComponent->UpdatedComponent = RootComponent;

	//MagneticMaterial = CreateDefaultSubobject<UMaterial>(TEXT("MagneticMaterial"));
	//NonMagneticMaterial = CreateDefaultSubobject<UMaterial>(TEXT("NonMagneticMaterial"));

	bIsLaunched = false;
	bCheckCubeVelocity = false;
}

// sets launch direction to be straight down
void ACubePawn::SetLaunchDirectionDown()
{
	CurrentLaunchDirection.Set(0, 0, -1);
}

void ACubePawn::HitReceived(FVector initLoc)
{
	FVector tempVec;
	FVector currLoc = GetActorLocation();
	FVector HitDirection = initLoc - currLoc;
	HitDirection.Normalize();
	UE_LOG(LogTemp, Warning, TEXT("Hitdirection: %s"), *HitDirection.ToString());
	if (!bIsLaunched)
	{
		tempVec = FindNearestDirection(HitDirection.X, HitDirection.Y);
		UE_LOG(LogTemp, Warning, TEXT("Tempvec after findnearest: %s"), *tempVec.ToString());
	}
	tempVec.Z = 0;
	if (!bIsMagnetic)
	{
	tempVec = tempVec * -1;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Direction the cube was launched: %s"), *tempVec.ToString());

	CurrentLaunchDirection = tempVec;
	bIsLaunched = true;
	bCubeMoved = true;
}

void ACubePawn::AddDownWardForce()
{
	FVector Start = GetActorLocation() + FVector(0, 0, -100);
	FVector End = GetActorLocation() + FVector(0, 0, -1000);
	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	
	// Line trace to look for actors below the cube
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	// Reference to an actor we hit
	AActor* HitActor = Hit.GetActor();

	// Visualising the line
	DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);
	if (bHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
		if (Hit.Distance > 50) // 56 is the max threshold for this to work atm, above and the block will just keep going. If we change the size of the tiles this need to be redone.
		{
			bIsLaunched = false;
			MovementComponent->StopMovementImmediately();
			FVector TargetLoc = Cast<ALevelTile>(HitActor)->GetActorLocation();
			FVector CurrentLoc = GetActorLocation();
			FVector NewLoc(TargetLoc.X, TargetLoc.Y, CurrentLoc.Z); 
			SetActorLocation(NewLoc, false); // Probably a better way to do this.
			SetLaunchDirectionDown();
			bIsLaunched = true;
		}
	}
}

ECollisionChannel ACubePawn::GetCollisionChannel(AActor* cube)
{
	ECollisionChannel temp = CubeMesh->UPrimitiveComponent::GetCollisionObjectType();
	return temp;
}

bool ACubePawn::GetIsMagnetic()
{
	return bIsMagnetic;
}

// Finds the closest cardinal direction the cube will be launched in.
FVector ACubePawn::FindNearestDirection(float Xin, float Yin)
{
	float tempX = Xin;
	float tempY = Yin;
	float tempXlength;
	float tempYlength;
	bool Xnegative{ false };
	bool Ynegative{ false };
	FVector tempVec;
	
	if (FMath::FloorToInt(tempX) == 0 && FMath::FloorToInt(tempY) == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bad direction: %s"), *tempVec.ToString());
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
			tempVec.Z = 0;
			return tempVec;
		}
		else
		{
			tempVec.X = 1;
			tempVec.Y = 0;
			tempVec.Z = 0;
			return tempVec;
		}
	}
	else
	{
		if (Ynegative == true)
		{
			tempVec.X = 0;
			tempVec.Y = -1;
			tempVec.Z = 0;
			return tempVec;
		}
		else
		{
			tempVec.X = 0;
			tempVec.Y = 1;
			tempVec.Z = 0;
			return tempVec;
		}
	}
}

void ACubePawn::MoveCube()
{
	if (bIsLaunched)
	{
		FVector tempVec = CurrentLaunchDirection * BaseLaunchVelocity;
		float DeltaSeconds = GetWorld()->GetDeltaSeconds();
		tempVec = tempVec * DeltaSeconds;

		MovementComponent->AddInputVector(tempVec, true);
		MoveCubeDoOnce();
		
	}
	else
	{
		CheckCubeVelocityDoOnce();
	}
}

void ACubePawn::CheckForBoundaryHit()
{
	if (bCheckCubeVelocity)
	{
		FVector tempVec;
		float vecSize;
		tempVec = CubeMesh->GetPhysicsLinearVelocity();

		// UE_LOG(LogTemp, Warning, TEXT("This is the Vector from Cube: %s"), *tempVec.ToString());

		vecSize = tempVec.Size();

		// UE_LOG(LogTemp, Warning, TEXT("vecSize is: %f"), vecSize);
		if (vecSize <= 0)
		{
			AlbertCharacter->SetOverlapTrue();
			UE_LOG(LogTemp, Warning, TEXT("Cube no longer moving!"));
			bIsLaunched = false;
		}	
	}	
}

void ACubePawn::MoveCubeDoOnce()
{
	if (bCubeMoved)
	{
		bCubeMoved = false;
		GetWorld()->GetTimerManager().SetTimer(CubeDelayTimerHandle, this, &ACubePawn::ResetCheckCubeVelocity, 0.1f, false);
		return;
	}
	else
	{
		return;
	}
}

void ACubePawn::ResetMoveCubeDoOnce()
{
	bCubeMoved = true;
}

void ACubePawn::CheckCubeVelocityDoOnce()
{
	if (bCheckCubeVelocity)
	{
		bCheckCubeVelocity = false;
		ResetMoveCubeDoOnce();
		return;
	}
	else
	{
		return;
	}
}

void ACubePawn::ResetCheckCubeVelocity()
{
	bCheckCubeVelocity = true;
}

// Called when the game starts or when spawned
void ACubePawn::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorTransform();
	AlbertCharacter = Cast<AAlbert_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (bIsMagnetic)
	{
		//CubeMesh->SetMaterial(0, MagneticMaterial); //TODO: Redo
		CubeMesh->SetCollisionObjectType(COLLISION_MAGNETICCUBE);
	}
	else
	{
		//CubeMesh->SetMaterial(0, NonMagneticMaterial); //TODO: Redo
	}
}

// Called every frame
void ACubePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddDownWardForce();
	MoveCube();
	CheckForBoundaryHit();
}

// Called to bind functionality to input
void ACubePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Super::SetupPlayerInputComponent(PlayerInputComponent);

}

