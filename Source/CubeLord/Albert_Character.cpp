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
#include "DrawDebugHelpers.h"
#include "Leveltile.h"

#define COLLISION_MAGNETICCUBE ECC_GameTraceChannel2

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

	// Sound1 = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound1"));
	// Sound1->SetupAttachment(GetCapsuleComponent());
	// Sound2 = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound2"));
	// Sound2->SetupAttachment(GetCapsuleComponent());

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
	CubeVolume->SetGenerateOverlapEvents(false);


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
	CubeVolume->OnComponentBeginOverlap.AddDynamic(this, &AAlbert_Character::OnOverlap);
	CubeVolume->OnComponentEndOverlap.AddDynamic(this, &AAlbert_Character::OnOverlapEnd);

	//	Simple way of setting cameralocation relative to the player start
	//		NEEDS TO BE CHANGED LATER
	CamLocation = GetActorLocation() + FVector(200.0f, 0.0f, 0.0f);

	if (bAltControls)
	{
		CubeVolume->SetGenerateOverlapEvents(true);
	}
}

// Called every frame
void AAlbert_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveCamera();
	// RotateCamera();

	CameraParentRotation = CameraRoot->GetComponentRotation();

	CollisionUnderPlayerCheck();

	//	RayTracing to check what is beneath the player
	RayTraceFromSocket(4.0f, "BoneSocket");
}

// Called to bind functionality to input
void AAlbert_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AAlbert_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAlbert_Character::MoveRight);
	PlayerInputComponent->BindAction("ResetLevel", IE_Pressed, this, &AAlbert_Character::ResetLevel);
	//PlayerInputComponent->BindAction("HammerSwing", IE_Pressed, this, &AAlbert_Character::HammerSwing);

	PlayerInputComponent->BindAction("HammerSwing", IE_Pressed, this, &AAlbert_Character::StartAttacking);
	PlayerInputComponent->BindAction("HammerSwing", IE_Released, this, &AAlbert_Character::StopAttacking);

	PlayerInputComponent->BindAction("MagnetPull", IE_Pressed, this, &AAlbert_Character::MagneticPull);
	// PlayerInputComponent->BindAction("MagnetPull", IE_Released, this, &AAlbert_Character::StopPulling);
	FInputActionBinding& Toggle = PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AAlbert_Character::PauseGame);
	Toggle.bExecuteWhenPaused = true;

	PlayerInputComponent->BindAction("Testing", IE_Pressed, this, &AAlbert_Character::TESTING);
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
	bDeath = true;
	CallDeath();
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
		CubeVolume->SetGenerateOverlapEvents(true);
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
		CubeVolume->SetGenerateOverlapEvents(false);
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
	isPulling = false;
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
	DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f, 0, 5.0f);
		if (bHit && HitActor->IsA(ACubePawn::StaticClass()))
		{
			if(Cast<ACubePawn>(HitActor)->GetIsMagnetic() == true)
			{
				DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
				FVector CurrentLoc = GetMesh()->GetComponentLocation();
				FVector EndLoc = CurrentLoc + GetMesh()->GetForwardVector() * 200;
				Cast<ACubePawn>(HitActor)->HitReceived(EndLoc);
			}
			else
			{
				DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
				FVector CurrentLoc = GetMesh()->GetComponentLocation();
				Cast<ACubePawn>(HitActor)->HitReceived(CurrentLoc);
			}
		}
	}
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AAlbert_Character::StopAttacking, 1.f, false);
}

void AAlbert_Character::MagneticPull()
{
	if(!isPulling)
	{
	isPulling = true;
	FVector Start = GetCapsuleComponent()->GetComponentLocation() + FVector(0,0,-50);
	FVector End = Start + GetMesh()->GetForwardVector() * 2000;
	
	FHitResult Hit;
	FCollisionQueryParams TraceParams;

	// Line trace to look for magnetic cubes.
	bool bHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, COLLISION_MAGNETICCUBE, TraceParams);

	// Reference to an actor we hit
	AActor* HitActor = Hit.GetActor();
	
	// Visualising the line
	DrawDebugLine(GetWorld(), Start, End, FColor::Orange, false, 2.0f, 0, 5.0f);
		if (bHit)
		{		
				DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);		
				FVector MagnetLoc = GetMesh()->GetComponentLocation();
				Cast<ACubePawn>(HitActor)->HitReceived(MagnetLoc);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &AAlbert_Character::StopPulling, 1.f, false);
}

// Old Attack
void AAlbert_Character::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bAltControls)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Enemy Overlaps %s"), *OtherActor->GetName())
		if (bCanOverlap)
		{
			if (OtherActor->IsA(ACubePawn::StaticClass()))
			{
				FVector CurrentLoc = GetCapsuleComponent()->GetComponentLocation();
				Cast<ACubePawn>(OtherActor)->HitReceived(CurrentLoc);
				bCanOverlap = false;
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

	// Line trace to look for actors below the cube
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	// Reference to an actor we hit
	AActor* HitActor = Hit.GetActor();

	// Visualising the line
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.5f, 0, 5.0f);

	if (HitActor->IsA(ALevelTile::StaticClass()))
	{
		if(Hit.bBlockingHit)
		{
			// Sets the first tiles pointer
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
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
		TraceParams
	);
	
	return Hit;
}


void AAlbert_Character::RayTraceFromSocket(float Range, FName SocketName) 
{
	FHitResult HitResult = RayTracer(Range, SocketName);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		if (!bActorHit)
		{
			if (ActorHit->ActorHasTag(TEXT("Block")))
			{
				// UE_LOG(LogTemp, Warning, TEXT("Hits Floor"));
				PlayEffect(Particle2);
				PlaySound(Sound1, SocketName);
				// UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound1, GetMesh()->GetSocketLocation(SocketName));
			}
			else
			{
				// UE_LOG(LogTemp, Warning, TEXT("Hits Dirt"));
				PlayEffect(Particle1);
				PlaySound(Sound2, SocketName);
				// UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound2, GetMesh()->GetSocketLocation(SocketName));
			}
		}
		bActorHit = true;
	}
	else
	{
		if (bActorHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Actor Hit"));
		}
		bActorHit = false;
	}
}

void AAlbert_Character::PlayEffect(UParticleSystem* ParticleToPlay) 
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleToPlay, GetMesh()->GetSocketLocation("BoneSocket"));
}

void AAlbert_Character::PlaySound(USoundBase* SoundToPlay, FName SocketName) 
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetMesh()->GetSocketLocation(SocketName));
}

void AAlbert_Character::TESTING() 
{
	// GameModeRef->StartGame();
	GameModeRef->TitleScreen(true);
	// GameModeRef->GoToTitleScreen(true);
}

