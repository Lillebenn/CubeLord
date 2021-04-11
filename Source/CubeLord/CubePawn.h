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

	/**True if the cube is magnetic and can be pulled thowards the player, false otherwise*/
	UPROPERTY(Category = Cube, EditAnywhere)
	bool bIsMagnetic{ false };

	/**Material that the cube uses if it is magnetic*/
	UPROPERTY(EditAnywhere)
	class UMaterial* MagneticMaterial;

	/**Material that the cube uses if it is NOT magnetic*/
	UPROPERTY(EditAnywhere)
	class UMaterial* NonMagneticMaterial;

	/**True if the cube is moving, false otherwise*/
	bool bIsLaunched{ false };

	/**Helper variable to determine when a function can and cant run*/
	bool bCheckCubeVelocity{ false };

	/**True if the Cube has enough speed to kill an enemy on impact, false otherwise*/
	bool bCubeAboveThresholdSpeed{ false };

	/**Helper variable to determine when a function can and cant run*/
	bool bCubeMoved{ false };

	/**True if the cube is falling downwards, false otherwise*/
	bool bIsFalling{ false };

	/**What direction the cube is being launced*/
	FVector CurrentLaunchDirection;

	/**Sets the launch direction directly downwards, used to make the cube fall into holes*/
	void SetLaunchDirectionDown();

	/**The speed at which the cube is launched*/
	float BaseLaunchVelocity{ 300 };

	/**What happens when the cube is hit*/
	void HitReceived(FVector initLoc);

	/**Pulls the cube down when it's above a hole*/
	void AddDownWardForce();


private:
	
	/**Used to find which direction (X or Y) the cube should travel in when hit*/
	FVector FindNearestDirection(float Xin, float Yin);

	/**Moves the cube*/
	void MoveCube();

	/**Helper function that determines when the cube is no longer moving, resets variables accordingly*/
	void CheckForBoundaryHit();

	/**Initial spawn of the cube, so it can be reset*/
	FTransform InitialLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**Handles the timer between cube launches*/
	FTimerHandle CubeDelayTimerHandle;

	/**Helper Function that runs the check for if the cube is moving on a delay*/
	void MoveCubeDoOnce();

	/**Helper Function that that resets bCubeMoved*/
	void ResetMoveCubeDoOnce();

	/**Helper Function that resets bCheckCubeVelocity and runs ResetMoveCubeDoOnce*/
	void CheckCubeVelocityDoOnce();
	
	/**Helper Function that resets bCheckCubeVelocity*/
	void ResetCheckCubeVelocity();

	/**Component that allows our cubes to move*/
	UFloatingPawnMovement* MovementComponent;

public:	
	/**Pointer to the games main character*/
	AAlbert_Character* AlbertCharacter;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
