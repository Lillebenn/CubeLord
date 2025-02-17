// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LevelTile.h"
#include "Albert_Character.generated.h"

class USceneComponent;
class UParticleSystem;
class USoundBase;
class UMaterialInstanceDynamic;

UCLASS()
class CUBELORD_API AAlbert_Character : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USoundBase* DeathSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HammerMesh;

	 	
	// struct FSoundAttenuationSettings* SoundAtt;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USoundAttenuation* SoundAtt;
	

	/** Overlap volume to check for possible cube targets */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CubeVolume;

	/**Reference to the gamemode*/
	class ACubeLordGameMode* GameModeRef;

	/***/
	UPROPERTY(EditAnywhere)
	AActor* CameraPositionActor{ nullptr };

	/**The current number of moves used in a level*/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MovesUsed{ 0 };

	/**The Cameras current Yaw position*/
	float CurrentYaw{ 0.0f };

	/**The Cameras target Yaw position*/
	float TargetYaw{ 0.0f };

	/**The Cameras Location*/
	FVector CamLocation;

	/**The Cameras parent rotation*/
	FRotator CameraParentRotation;

	/**True if the character is attacking, false otherwise*/
	bool isAttacking{ false };

	/**True if the character is pulling a magnetic cube, false otherwise*/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsPulling{ false };

	/**True if the player can use overlap events to push the cube, false otherwise*/
	bool bCanOverlap{ true };

	/**True if the Character is walking on a surface that can produce sound / effect, false otherwise*/
	bool bActorHit{ false };

	/**True if the character is not looking diagonally, false otherwise*/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsNotDiagonal{ true };

	/**Rotates the camera*/
	void RotateCamera();

	/**Moves the Camera*/
	void MoveCamera();

	/**Resets the current level*/
	void ResetLevel();

	/**Pauses the game*/
	UFUNCTION(BlueprintCallable)
	void PauseGame();

	/**Old version of pushing the cube*/
	UFUNCTION(BlueprintCallable)
	void StartAttacking();
	/**OLD stops pushing the cube*/
	UFUNCTION(BlueprintCallable)
	void StopAttacking();

	/**Sets bIsPulling to false*/
	void StopPulling();

	/**Pushes a Cube that is infront of the character away*/
	void HammerSwing();

	/**Pulls a magnetic cube infront of the character thowards the character*/
	UFUNCTION(BlueprintCallable)
	void MagneticPull();

	/**Plays an effect at a location I.E Dust clound on footsteps*/
	void PlayEffect(UParticleSystem* ParticleToPlay);

	/**Plays a sound at a location*/
	void PlaySound(USoundBase* SoundToPlay, FName SocketName);

	/**Checks the players current rotation, and if it's 0, 90, 180 or 270 it sets bIsNotDirectional to true*/
	void CheckCurrentRotation();

	/**Linetrace to look for a magnetic cube in range. If it hits one, it makes the hammer glow*/
	void ScanForMagneticCube();

	/**Dynamic Material Instance used to change the appearance of the Hammer if a magnetic cube is in range*/
	UMaterialInstanceDynamic* DynamicMaterial;

	/**Pointer to the enemy character "Haunted Armor"*/
	class AArmorPawn* Armor;
	UPROPERTY(EditAnywhere)
	AActor* ArmorActor;
	
	/**True if the character is dead, false otherwise*/
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bDeath{ false };

	
	//	Letting the player skip the current level
	void SkipLevel();
	AActor* LT;
	class ALevelTransitioner* LTclass;

public:
	// Sets default values for this character's properties
	AAlbert_Character();

	/**Bool for if the player should use the alternative controls (Old, collision overlap) to push the cubes.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAltControls{ false };

	/**Bool to check if player is aiming at a magnetic cube.*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bMagneticCubeDetected{ false };

	/**The current cube being overlapped by the characters CubeVolume*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ACubePawn* CurrentOverlappingCubePawn;

	/**Where the cube is right now*/
	FVector CurrentCubeLocation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// OLD
	UFUNCTION()
	void OnHammerheadOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnHammerHitFX(FTransform Transform);

	UFUNCTION(BlueprintImplementableEvent)
	void EventStopHammerPullBlueprint(float Delay);

	UFUNCTION(BlueprintImplementableEvent)
	void EventSpawnMagneticPullFX(AActor* InCubeHit);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**Checks for a Leveltile under the player, sets that one tile to block magnetic cubes so the player can't be pushed by pulling a cube to thowards the player*/
	void CollisionUnderPlayerCheck();

	/**Helper function for pushing cubes*/
	void SetOverlapTrue();

	/**Called if they player dies by running into an enemy*/
	void Death();

	/**Increases MovesUsed by 1*/
	void IncreaseMoves();
		
	//	Raytracer to be used anywhere on Albert. Needs a Socket on the skeletal mesh you want to raytrace from
	FHitResult RayTracer(float Range, FName SocketName);	
	FName RayTraceFromSocket(float Range, FName SocketName);

protected:

	/**Pointer to the current tile the player is standing on*/
	ALevelTile* CurrentLevelTile;

	/**Handles the timer between cube launches*/
	FTimerHandle CooldownTimerHandle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**Moves the character forward or backwards*/
	void MoveForward(float Value);

	/**Moves the character Left or Right*/
	void MoveRight(float Value);

	UFUNCTION(BlueprintImplementableEvent)
	void CallDeath();
};
