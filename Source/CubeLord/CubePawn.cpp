// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePawn.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "LevelTile.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"


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
	FVector HitDirection = FVector(initLoc.X, initLoc.Y, 0.f) - FVector(currLoc.X, currLoc.Y, 0.f);
	HitDirection.Normalize();

	UE_LOG(LogTemp, Warning, TEXT("Hitdirection: %s"), *HitDirection.ToString());

	if (!bIsLaunched)
	{
		tempVec = FindNearestDirection(HitDirection.X, HitDirection.Y);
		UE_LOG(LogTemp, Warning, TEXT("Tempvec after findnearest: %s"), *tempVec.ToString());


		tempVec.Z = 0;
		if (!bIsMagnetic || bMagneticHit)
		{
			tempVec = tempVec * -1;
		}

		UE_LOG(LogTemp, Warning, TEXT("Direction the cube was launched: %s"), *tempVec.ToString());

		CurrentLaunchDirection = tempVec;
		bIsLaunched = true;
		bCubeMoved = true;
		bMagneticHit = false;
	}

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
		if (!bIsFalling)
		{
			if (Hit.Distance > 50)
			{
				bIsFalling = true;
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
}

void ACubePawn::AlignmentCheck()
{
	FVector Start = GetActorLocation() + FVector(0, 0, -100);
	FVector End = GetActorLocation() + FVector(0, 0, -200);
	FHitResult Hit;
	FCollisionQueryParams TraceParams;

	// Line trace to look for actors below the cube
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	// Reference to an actor we hit
	AActor* HitActor = Hit.GetActor();

	// Visualising the line
	DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f);
	if (bHit && HitActor->IsA(ALevelTile::StaticClass()))
	{
		FVector TargetLoc = Cast<ALevelTile>(HitActor)->GetActorLocation();
		FVector CurrentLoc = GetActorLocation();
		FVector NewLoc(TargetLoc.X, TargetLoc.Y, CurrentLoc.Z);
		SetActorLocation(NewLoc, false);
		UE_LOG(LogTemp, Warning, TEXT("Cube realigned!"));
	}
}

ECollisionChannel ACubePawn::GetCollisionChannel(AActor* cube)
{
	ECollisionChannel temp = CubeMesh->UPrimitiveComponent::GetCollisionObjectType();
	return temp;
}

void ACubePawn::SetMagneticHit()
{
	bMagneticHit = true;
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

	//UE_LOG(LogTemp, Warning, TEXT("Hitdirection: %d, %d"), Xin, Yin);

	if (FMath::RoundToInt(tempX) == 0 && FMath::RoundToInt(tempY) == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bad direction: %s, X=%f Y=%f"), *tempVec.ToString(), tempX, tempY);
		bIsLaunched = false;
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
			AlbertCharacter->SetOverlapTrue(); // TODO replace with animationLoop
			bIsLaunched = false;
			bIsFalling = false;
			UE_LOG(LogTemp, Warning, TEXT("Cube no longer moving!"));	
			AlignmentCheck();
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
	auto Cube = FindComponentByClass<UStaticMeshComponent>();
	auto Material = Cube->GetMaterial(0);

	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	Cube->SetMaterial(0, DynamicMaterial);

	InitialLocation = GetActorTransform();
	AlbertCharacter = Cast<AAlbert_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (bIsMagnetic)
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), 1); // Lerp blend, 0 = marble, 1 = Nickel
		DynamicMaterial->SetScalarParameterValue(TEXT("RoughnessBlend"), 0.25); // Roughness
		DynamicMaterial->SetScalarParameterValue(TEXT("MetallicBlend"), 0.8); // Metallic
		DynamicMaterial->SetScalarParameterValue(TEXT("Marble"), 0); // sets it below 0.5 so that it uses the metallic normal map

		GridCollision->SetCollisionObjectType(COLLISION_MAGNETICCUBE);
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

