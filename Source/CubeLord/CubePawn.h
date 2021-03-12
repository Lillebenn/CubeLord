// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	// Used to check if the cube has been hit by the player.
	bool bIsLaunched{ false };

	FVector CurrentLaunchDirection;

	float BaseLaunchVelocity{ 300 };

	void HitReceived(FVector initLoc);

private:

	/* Used to find which direction (X or Y) the cube should travel in when hit*/
	FVector FindNearestDirection(float Xin, float Yin);

	void MoveCube();

	FTransform InitialLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFloatingPawnMovement* MovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
