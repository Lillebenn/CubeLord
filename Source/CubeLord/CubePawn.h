// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Albert_Character.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Engine/StaticMesh.h"
#include "Components/PrimitiveComponent.h"
#include "CubePawn.generated.h"

class UMaterialInstanceDynamic;

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

	//	Will play sound when cube is launched and is stopped
	UFUNCTION(BlueprintImplementableEvent)
	void PlaySound();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlaySoundAtStop();

	/**True if the cube is magnetic and can be pulled thowards the player, false otherwise*/
	//UPROPERTY(VisibleAnywhere, Category = Cube) //, EditAnywhere
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Cube)
	bool bIsMagnetic{ false };

	/**True if the cube is moving, false otherwise*/
	UPROPERTY(BlueprintReadOnly)
	bool bIsLaunched{ false };

	/**Helper variable to determine when a function can and cant run*/
	bool bCheckCubeVelocity{ false };

	/**Helper variable to determine when a function can and cant run*/
	bool bCubeMoved{ false };

	/**True if the cube is falling downwards, false otherwise*/
	bool bIsFalling{ false };

	/**True if the magnetic cube should be pushed, false otherwise*/
	bool bMagneticHit{ false };

	/**Magnetic hit by Albert*/
	class AAlbert_Character* CurrentPlayerCharacter{ nullptr };

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

	/**Uses a Linetrace to make sure the cube is properly aligned with the leveltile grid*/
	void AlignmentCheck();

	/**Returns the cubes collision channel*/
	ECollisionChannel GetCollisionChannel(AActor* cube);

	/**Used to set bMagneticHit to true*/
	void SetMagneticHit(AAlbert_Character* InCharacter);

	/**Returns bIsMagnetic*/
	bool GetIsMagnetic();

private:
	
	/**Used to find which direction (X or Y) the cube should travel in when hit*/
	FVector FindNearestDirection(float Xin, float Yin);

	/**Moves the cube*/
	void MoveCube();

	/**Helper function that determines when the cube is no longer moving, resets variables accordingly*/
	void CheckForBoundaryHit();

	/**Initial spawn of the cube, so it can be reset*/
	FTransform InitialLocation;

	/**Dynamic Material Instance used to change the appearance of the magnetic cubes*/
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* SlideSound;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* MovementComponent;

public:	
	/**Pointer to the games main character*/
	AAlbert_Character* AlbertCharacter;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
