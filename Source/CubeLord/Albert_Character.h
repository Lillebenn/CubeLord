// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Albert_Character.generated.h"

class USceneComponent;
class UParticleSystem;
class USoundBase;

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
	UParticleSystem* Particle1{ nullptr };
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* Particle2{ nullptr };
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USoundBase* Sound1;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USoundBase* Sound2;
	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USoundBase* DeathSound;

	

	/** Overlap volume to check for possible cube targets */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CubeVolume;


	class ACubeLordGameMode* GameModeRef;

	UPROPERTY(EditAnywhere)
	AActor* CameraPositionActor{ nullptr };

	float CurrentYaw{ 0.0f };
	float TargetYaw{ 0.0f };
	FVector CamLocation;
	FRotator CameraParentRotation;
	bool isAttacking{ false };
	bool isPulling{ false };
	bool bCanOverlap{ true };

	bool bActorHit{ false };

	void RotateCamera();
	void MoveCamera();

	void ResetLevel();
	void PauseGame();

	// OLD
	void StartAttacking();
	void StopAttacking();

	void StopPulling();

	// Function that pushes a block away from the player.
	void HammerSwing();

	// Function that pulls a magnetic cube thowards the player.
	void MagneticPull();

	//	Raytracer to be used anywhere on Albert. Needs a Socket on the skeletal mesh you want to raytrace from
	FHitResult RayTracer(float Range, FName SocketName);	
	void RayTraceFromSocket(float Range, FName SocketName);
	void PlayEffect(UParticleSystem* ParticleToPlay);
	void PlaySound(USoundBase* SoundToPlay, FName SocketName);

	void TESTING();
	class AArmorPawn* Armor;
	UPROPERTY(EditAnywhere)
	AActor* ArmorActor;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bDeath{ false };

public:
	// Sets default values for this character's properties
	AAlbert_Character();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAltControls{ false };

	class ACubePawn* CurrentOverlappingCubePawn;
	FVector CurrentCubeLocation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// OLD
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetOverlapTrue();

	void Death();
	
protected:

	/**Handles the timer between cube launches*/
	FTimerHandle CooldownTimerHandle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(BlueprintImplementableEvent)
	void CallDeath();
};
