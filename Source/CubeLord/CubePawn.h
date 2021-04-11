// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Albert_Character.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Engine/StaticMesh.h"
#include "CubePawn.generated.h"

UCLASS()
class CUBELORD_API ACubePawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* GridCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CubeMesh;

public:
	// Sets default values for this pawn's properties
	ACubePawn();

	// Sets if the cube is magnetic or not.
	bool bIsMagnetic{ false };

	/**True if the cube has been launched in a direction, false otherwise*/
	bool bIsLaunched{ false };

	bool bCheckCubeVelocity{ false };

	/**True if the Cube has enough speed to kill an enemy on impact, false otherwise*/
	bool bCubeAboveThresholdSpeed{ false };

	bool bCubeMoved{ false };

	/**True if the cube is falling downwards, false otherwise*/
	bool bIsFalling{ false };

	FVector CurrentLaunchDirection;

	void SetLaunchDirectionDown();

	float BaseLaunchVelocity{ 300 };

	void HitReceived(FVector initLoc);

	void AddDownWardForce();


private:

	/**Used to find which direction (X or Y) the cube should travel in when hit*/
	FVector FindNearestDirection(float Xin, float Yin);

	/**Moves the cube*/
	void MoveCube();

	void CheckForBoundaryHit();

	/**Initial spawn of the cube, so it can be reset*/
	FTransform InitialLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**Handles the timer between cube launches*/
	FTimerHandle CubeDelayTimerHandle;

	FTimerHandle GravityDelayTimerHandle;

	void MoveCubeDoOnce();

	void ResetMoveCubeDoOnce();

	void CheckCubeVelocityDoOnce();
	
	/**Sets bCheckCubeVelocity back to false*/
	void ResetCheckCubeVelocity();

	/**Component that allows our cubes to move*/
	UFloatingPawnMovement* MovementComponent;

public:	

	AAlbert_Character* AlbertCharacter;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
