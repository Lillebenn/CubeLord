// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "LevelTile.generated.h"

UCLASS()
class CUBELORD_API ALevelTile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* TileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* AboveGroundCollision;

public:	
	// Sets default values for this actor's properties
	ALevelTile();
	
	// Old version of setting the collision responses. Could set several tiles to block the cubes, and could cause issues if the character overlapped with a tile that had a magnetic cube on it.

	/**If AboveGroundCollision is overlapping with the main character it sets the collision channel to block magnetic cubes so they can't hit the player*/
	/*UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);*/

	/**Resets the collision channel so magnetic cubes can once again enter the collider if they player isn't standing in it*/
	/*UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) 
			*/


	/**Sets the collision response of the leveltile the character is standing on to block magnetic cubes*/
	void SetBlockResponse();
	
	/**Sets the collision response of the leveltile the character left to once again overlap magnetic cubes*/
	void ResetCollisionResponse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
