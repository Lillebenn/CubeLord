// Fill out your copyright notice in the Description page of Project Settings.


#include "Albert_Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CubePawn.h"
#include "CubeLordGameMode.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"
#include "Leveltile.h"
#include "LevelTransitioner.h"

#define COLLISION_MAGNETICCUBE ECC_GameTraceChannel2
#define COLLISION_FLOOR ECC_GameTraceChannel3

// Sets default values
AAlbert_Character::AAlbert_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	GetCapsuleComponent()->SetupAttachment(Root);
	GetCapsuleComponent()->InitCapsuleSize(35.0f, 92.0f);
	GetCapsuleComponent()->SetWorldLocation(FVector(0.0f, 0.0f, 92.0f));

	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	CameraRoot->SetupAttachment(Root);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(CameraRoot);
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CubeVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrace"));
	CubeVolume->SetupAttachment(GetCapsuleComponent());
	CubeVolume->SetGenerateOverlapEvents(true);

	HammerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HammerMesh"));
	HammerMesh->SetupAttachment(ACharacter::GetMesh(), "Bone");

	//	Don't rotate when the controller rotates. 
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//	Kopiert rett fra UE sin default 3rd person character
	GetCharacterMovement()->bOrientRotationToMovement = true;	//	Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 800.0f, 0.0f);	// ... at this rotation rate

}

// Called when the game starts or when spawned
void AAlbert_Character::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<ACubeLordGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	HammerMesh->OnComponentBeginOverlap.AddDynamic(this, &AAlbert_Character::OnHammerheadOverlap);
	HammerMesh->OnComponentEndOverlap.AddDynamic(this, &AAlbert_Character::OnOverlapEnd);

	//	Simple way of setting cameralocation relative to the player start
	//		NEEDS TO BE CHANGED LATER
	CamLocation = GetActorLocation() + FVector(200.0f, 0.0f, 0.0f);

	if (bAltControls)
	{
		CubeVolume->SetGenerateOverlapEvents(true);
	}

	HammerMesh->SetupAttachment(ACharacter::GetMesh(), "Bone");
	auto Material = HammerMesh->GetMaterial(1);
	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	HammerMesh->SetMaterial(1, DynamicMaterial);
	StopAttacking();
}

// Called every frame
void AAlbert_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCamera();

	CameraParentRotation = CameraRoot->GetComponentRotation();

	CheckCurrentRotation();
	ScanForMagneticCube();

	CollisionUnderPlayerCheck();
}

// Called to bind functionality to input
void AAlbert_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AAlbert_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAlbert_Character::MoveRight);
	PlayerInputComponent->BindAction("ResetLevel", IE_Pressed, this, &AAlbert_Character::ResetLevel);
	PlayerInputComponent->BindAction("MagnetPull", IE_Pressed, this, &AAlbert_Character::MagneticPull);

	FInputActionBinding& Toggle = PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AAlbert_Character::PauseGame);
	Toggle.bExecuteWhenPaused = true;

	PlayerInputComponent->BindAction("SkipLevel", IE_Pressed, this, &AAlbert_Character::SkipLevel);
}

void AAlbert_Character::MoveForward(float Value) 
{
	if ((Controller != NULL) && (Value != 0.f))
	{
		//	Finds out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw + CameraParentRotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAlbert_Character::MoveRight(float Value) 
{
	if ((Controller != NULL) && (Value != 0.f))
	{
		//	Finds out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw + CameraParentRotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AAlbert_Character::SetOverlapTrue()
{
	bCanOverlap = true;
}

void AAlbert_Character::Death()
{
	if (!bDeath)
	{
		bDeath = true;
		CallDeath();
	}
}

void AAlbert_Character::RotateCamera()
{
	FRotator Rotation = CameraRoot->GetComponentRotation();
	Rotation.Yaw = TargetYaw;
	CameraRoot->SetWorldRotation(Rotation);
}

void AAlbert_Character::MoveCamera() 
{
	// CamLocation = FVector(0.0f, 50.0f, 30.0f);
	CameraRoot->SetWorldLocation(CamLocation);
}

void AAlbert_Character::ResetLevel() 
{
	
	GameModeRef->ResetLevelFunc();
}

void AAlbert_Character::PauseGame() 
{
	GameModeRef->PauseGameFunc();
}

// Old Attack
void AAlbert_Character::StartAttacking()
{
	if (!bAltControls)
	{
		HammerMesh->SetGenerateOverlapEvents(true);
		UE_LOG(LogTemp, Warning, TEXT("Albert Smash!"));
		isAttacking = true;
	}
	else
	{
		isAttacking = true;
		if (CurrentOverlappingCubePawn != nullptr)
		{
			Cast<ACubePawn>(CurrentOverlappingCubePawn)->SetMagneticHit();
			Cast<ACubePawn>(CurrentOverlappingCubePawn)->HitReceived(CurrentCubeLocation);
		}		
	}
}

// Stop Attacking
void AAlbert_Character::StopAttacking()
{
	if (!bAltControls)
	{
		HammerMesh->SetGenerateOverlapEvents(false);
		isAttacking = false;
	}
	else
	{
		isAttacking = false;
	}
}

// Stop Pulling
void AAlbert_Character::StopPulling()
{
	bIsPulling = false;
}

void AAlbert_Character::HammerSwing()
{
	if (!isAttacking)
	{
	isAttacking = true;
	FVector Start = GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, -50);
	FVector End = Start + GetMesh()->GetForwardVector() * 150;

	FHitResult Hit;
	FCollisionQueryParams TraceParams;

	// Line trace to look for magnetic cubes.
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	// Reference to an actor we hit
	AActor* HitActor = Hit.GetActor();

	// Visualising the line
	// DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f, 0, 5.0f);
		if (bHit && HitActor->IsA(ACubePawn::StaticClass()))
		{
			if(Cast<ACubePawn>(HitActor)->GetIsMagnetic() == true)
			{
				// DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
				FVector CurrentLoc = GetMesh()->GetComponentLocation();
				FVector EndLoc = CurrentLoc + GetMesh()->GetForwardVector() * 200;
				Cast<ACubePawn>(HitActor)->HitReceived(EndLoc);
			}
			else
			{
				// DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
				FVector CurrentLoc = GetMesh()->GetComponentLocation();
				Cast<ACubePawn>(HitActor)->HitReceived(CurrentLoc);
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AAlbert_Character::StopAttacking, 1.f, false);
}

void AAlbert_Character::MagneticPull()
{
	if (bIsNotDiagonal)
	{
		if(!bIsPulling)
		{
		bIsPulling = true;
		FVector Start = GetCapsuleComponent()->GetComponentLocation() + FVector(0,0,-50);
		FVector End = Start + GetMesh()->GetForwardVector() * 2000;
	
		FHitResult Hit;
		FCollisionQueryParams TraceParams;

		// Line trace to look for magnetic cubes.
		bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, COLLISION_MAGNETICCUBE, TraceParams);

		// Reference to an actor we hit
		AActor* HitActor = Hit.GetActor();
	
		// Visualising the line
		// DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f, 0, 5.0f);
			if (bHit)
			{		
					// DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);		
					FVector MagnetLoc = GetMesh()->GetComponentLocation();
					Cast<ACubePawn>(HitActor)->SetMagneticHit();
					Cast<ACubePawn>(HitActor)->HitReceived(MagnetLoc);
			}
		}
		// GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AAlbert_Character::StopPulling, 1.f, false);
		StopPulling();
		StopAttacking();
	}
}

void AAlbert_Character::OnHammerheadOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bAltControls)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Enemy Overlaps %s"), *OtherActor->GetName())
		if (bCanOverlap && OtherActor == CurrentOverlappingCubePawn)
		{
			if (OverlappedComponent == HammerMesh)
			{
				if (OtherActor->IsA(ACubePawn::StaticClass()))
				{
					FVector CurrentLoc = GetCapsuleComponent()->GetComponentLocation();
					SpawnHammerHitFX(FTransform(OverlappedComponent->GetComponentRotation(), SweepResult.Location, FVector(0.f, 0.f, 0.f)));
					Cast<ACubePawn>(OtherActor)->HitReceived(CurrentLoc);
					bCanOverlap = false;
				}
			}
		}
	}
	else
	{
		if (OtherActor->IsA(ACubePawn::StaticClass()))
		{
			CurrentCubeLocation = GetCapsuleComponent()->GetComponentLocation();
			CurrentOverlappingCubePawn = Cast<ACubePawn>(OtherActor);
		}		
	}
}

void AAlbert_Character::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CurrentOverlappingCubePawn != nullptr)
	{
		if (CurrentOverlappingCubePawn == OtherActor)
		{
			CurrentOverlappingCubePawn = nullptr;
		}
	}
}

void AAlbert_Character::CollisionUnderPlayerCheck()
{
	FVector Start = GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, -50);
	FVector End = Start + FVector(0, 0, -1000);
	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	FCollisionObjectQueryParams TraceObjectParams;
	TraceObjectParams.ObjectTypesToQuery = 0;
	TraceObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel3);
	

	// Line trace to look for actors below the cube
	//bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);
	bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, TraceObjectParams, TraceParams);

	// Reference to an actor we hit
	AActor* HitActor = Hit.GetActor();

	if(Hit.bBlockingHit)
	{
		// Visualising the line
		// DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.5f, 0, 5.0f);
		if (HitActor->IsA(ALevelTile::StaticClass()))
		{
			if (CurrentLevelTile == nullptr)
			{
				CurrentLevelTile = Cast<ALevelTile>(HitActor);
				Cast<ALevelTile>(CurrentLevelTile)->SetBlockResponse();
			}
			if (HitActor->IsA(ALevelTile::StaticClass()) && HitActor != CurrentLevelTile)
			{
				Cast<ALevelTile>(CurrentLevelTile)->ResetCollisionResponse(); // Reset old tile collision
				CurrentLevelTile = Cast<ALevelTile>(HitActor); // set new active tile
				Cast<ALevelTile>(CurrentLevelTile)->SetBlockResponse(); // Set new tile to block Cubes
			}
		}	
		else
		{
			if (CurrentLevelTile != nullptr)
			{
				Cast<ALevelTile>(CurrentLevelTile)->ResetCollisionResponse(); // Reset old tile collision
				CurrentLevelTile = nullptr; // reset pointer so that you can reactivate the same level tile after walking off it.
			}
		}
	}
	else
	{
		if (CurrentLevelTile != nullptr)
		{
			Cast<ALevelTile>(CurrentLevelTile)->ResetCollisionResponse(); // Reset old tile collision
			CurrentLevelTile = nullptr; // reset pointer so that you can reactivate the same level tile after walking off it.
		}
	}
}

//	Raycasting to beneath Alberts Capsule Component
FHitResult AAlbert_Character::RayTracer(float Range, FName SocketName) 
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		GetMesh()->GetSocketLocation(SocketName),
		GetMesh()->GetSocketLocation(SocketName) - FVector(0.0f, 0.0f, Range),
		FCollisionObjectQueryParams(ECollisionChannel::COLLISION_FLOOR),
		TraceParams
	);
	
	return Hit;
}

//	Raytracing from socket. Is called in the anim notify in the walking animation
FName AAlbert_Character::RayTraceFromSocket(float Range, FName SocketName) 
{
	FHitResult HitResult = RayTracer(Range, SocketName);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		if (ActorHit->ActorHasTag(TEXT("Block")))
		{
			return FName("Block");
		}
		else if (ActorHit->ActorHasTag(TEXT("Carpet")))
		{
			return FName("Carpet");
		}
		else
		{

			return FName("None");
		}
	}
	return FName("");
}

void AAlbert_Character::SkipLevel() 
{
	LT = UGameplayStatics::GetActorOfClass(GetWorld(), ALevelTransitioner::StaticClass());
	if (LT)
	{
		LTclass = Cast<ALevelTransitioner>(LT);

		FString LastMap = "Level_Final_WE";
		FString CurrentMap = GetWorld()->GetMapName();
		CurrentMap.RemoveFromStart (GetWorld()->StreamingLevelsPrefix);

		if (CurrentMap == LastMap)
		{
			LTclass->EndGame();
		}
		else
		{
			LTclass->BeginLevelTransition();
		}
	}

}

void AAlbert_Character::CheckCurrentRotation()
{
	FVector TempVec = GetCapsuleComponent()->GetComponentRotation().Vector();
	// UE_LOG(LogTemp, Warning, TEXT("Current Rotation: %s"), *TempVec.ToString());

	if (TempVec.X >= 0.98 || TempVec.X <= -0.98 || TempVec.Y >= 0.98 || TempVec.Y <= -0.98)
	{
		bIsNotDiagonal = true;
		// UE_LOG(LogTemp, Warning, TEXT("True"));
	}	
	else
	{
		bIsNotDiagonal = false;
	}
}

void AAlbert_Character::ScanForMagneticCube()
{
	FVector Start = GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, -50);
	FVector End = Start + GetMesh()->GetForwardVector() * 2000;

	FHitResult Hit;
	FCollisionQueryParams TraceParams;

	bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, COLLISION_MAGNETICCUBE, TraceParams);

	if (bHit)
	{
		
		if (bIsNotDiagonal)
		{
			DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveStrength"), 100);
			DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FVector(0.75f, 0.f, 0.f));
			//DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), 1); // Lerp blend, 0 = default, 1 = magnetic
			//DynamicMaterial->SetScalarParameterValue(TEXT("RoughnessBlend"), 0.25); // Roughness
			//DynamicMaterial->SetScalarParameterValue(TEXT("MetallicBlend"), 0.8); // Metallic
		}
		else
		{
			DynamicMaterial->SetVectorParameterValue(TEXT("Color"), FVector(0.f, 0.f, 0.75f));			
			DynamicMaterial->SetScalarParameterValue(TEXT("EmissiveStrength"), 1);
		}
		// UE_LOG(LogTemp, Warning, TEXT("Hit a cube!"));
	}
}


